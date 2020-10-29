#include "Connection.hpp"

#include <iostream>
#include <thread>

#include "api/Request_handler.hpp"

namespace eve
{
namespace server
{
Connection::Connection(boost::asio::ip::tcp::socket socket)
    : m_socket(std::move(socket)),
      m_signal_ready_close(),
      m_buffer(),
      m_parser(),
      m_request()
{
}

void Connection::open() { read(); }

void Connection::close()
{
  std::cout << "Closing connection..." << std::endl;
  if (m_socket.is_open())
  {
    m_socket.close();
  }
}

void Connection::on_ready_close(
    const boost::signals2::signal<
        void(const std::shared_ptr<Connection>& connection)>::slot_type& slot)
{
  m_signal_ready_close.connect(slot);
}

void Connection::read()
{
  auto self(shared_from_this());
  m_socket.async_read_some(
      boost::asio::buffer(m_buffer),
      [this, self](boost::system::error_code ec,
                   std::size_t bytes_transferred) {
        std::cout << "Reading data..." << std::endl;

        if (!ec)
        {
          const auto parse_result = m_parser.parse(
              m_buffer.data(), m_buffer.data() + bytes_transferred, m_request);
          switch (parse_result)
          {
            case eve::msg::Http_parser::Parse_result::Successful:
            {
              std::cout << m_request.to_string() << std::endl;
              api::Request_handler request_handler;
              write(request_handler.handle(m_request));
              break;
            }
            case eve::msg::Http_parser::Parse_result::Error:
              write(eve::msg::Reply::stock_reply(
                  eve::msg::Reply::Status_type::Internal_server_error));
              break;
            default:
              // Request is not done yet, continue reading.
              read();
          }
        }
        else if (ec != boost::asio::error::operation_aborted)
        {
          std::cerr << "Error reading: " << ec << std::endl;
          // close();
          this->m_signal_ready_close(self);
        }
      });
}

void Connection::write(const eve::msg::Reply& reply)
{
  auto self(shared_from_this());
  boost::asio::async_write(
      m_socket, reply.to_buffers(),
      [this, self](boost::system::error_code ec, std::size_t) {
        std::cout << "Writing data..." << std::endl;

        if (!ec)
        {
          std::cout << "Gracefully closing connection down..." << std::endl;
          boost::system::error_code ignored_ec;
          m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                            ignored_ec);
        }
        else
        {
          std::cerr << "Error writing: " << ec << std::endl;
        }

        if (ec != boost::asio::error::operation_aborted)
        {
          this->m_signal_ready_close(self);
        }
      });
}

}  // namespace server
}  // namespace eve