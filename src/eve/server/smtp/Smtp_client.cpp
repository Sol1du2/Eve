#include "Smtp_client.hpp"

#include "Smtp_code_expection.hpp"

namespace eve
{
namespace server
{
namespace smtp
{
namespace
{
constexpr int NUM_TRIES = 5;
}
Smtp_client::Smtp_client(const std::string& client_address,
                         const std::string& smtp_address,
                         const std::string& smtp_port)
    : m_io_context(1),
      m_socket(m_io_context),
      m_client_address(client_address),
      m_smtp_address(smtp_address),
      m_smtp_port(smtp_port)
{
}

void Smtp_client::send_message(const std::string& from_email,
                               const std::string& to_email,
                               const std::string& subject,
                               const std::string& body)
{
  boost::asio::ip::tcp::resolver resolver(m_io_context);
  boost::asio::ip::tcp::resolver::query query(m_smtp_address, m_smtp_port);
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
      resolver.resolve(query);
  boost::asio::connect(m_socket, endpoint_iterator);

  const auto close_socket = [&]() {
    boost::system::error_code ignored_ec;
    if (m_socket.is_open())
    {
      m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                        ignored_ec);
      m_socket.close(ignored_ec);
    }
  };

  try
  {
    read_smtp_message("220");

    send_smtp_message("HELO " + m_client_address + "\r\n", true, "250");
    send_smtp_message("MAIL FROM:<" + from_email + ">\r\n", true, "250");
    send_smtp_message("RCPT TO:<" + to_email + ">\r\n", true, "250");
    send_smtp_message("DATA\r\n", true, "354");
    send_smtp_message("Subject: " + subject + "\n\n" + body + "\r\n.\r\n", true,
                      "250");
    send_smtp_message("QUIT\r\n");
  }
  catch (const std::exception& exception)
  {
    close_socket();
    throw exception;
  }

  close_socket();
}

void Smtp_client::read_smtp_message(const std::string& expected_code)
{
  boost::asio::streambuf response;
  boost::asio::read_until(m_socket, response, "\n");

  std::string code;
  std::string message;

  std::istream is(&response);
  is >> code;

  while (!is.eof())
  {
    is >> message;
  }

  if (expected_code != code)
  {
    throw Smtp_code_exception(code + " " + message);
  }
}

void Smtp_client::send_smtp_message(const std::string& message,
                                    bool check_reply,
                                    const std::string& expected_code)
{
  boost::asio::streambuf write_buffer;
  std::ostream write_stream(&write_buffer);
  write_stream << message;
  boost::asio::write(m_socket, write_buffer);

  if (check_reply)
  {
    read_smtp_message(expected_code);
  }
}

void Smtp_client::send_smtp_message(const std::string& message)
{
  send_smtp_message(message, false, "");
}

}  // namespace smtp
}  // namespace server
}  // namespace eve