#ifndef EVE_SERVER_CONNECTION_HPP
#define EVE_SERVER_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include "../msg/Http_parser.hpp"
#include "../msg/Reply.hpp"
#include "../msg/Request.hpp"

namespace eve
{
namespace server
{
/// This class holds a connection to the server.
/// When the communication is done a signal is sent to inform about this. The
/// connection does not close the socket itself and close needs to be called.
class Connection : public std::enable_shared_from_this<Connection>
{
 public:
  Connection(boost::asio::ip::tcp::socket socket);

  Connection(const Connection&) = delete;

  Connection& operator=(const Connection&) = delete;

  void open();

  void close();

  /// This function is used to connect a slot to the signal when the connection
  /// is done with the work.
  void on_ready_close(
      const boost::signals2::signal<void(
          const std::shared_ptr<Connection>& connection)>::slot_type& slot);

 private:
  boost::asio::ip::tcp::socket m_socket;

  boost::signals2::signal<void(const std::shared_ptr<Connection>& connection)>
      m_signal_ready_close;

  std::array<char, 8192> m_buffer;
  eve::msg::Http_parser m_parser;
  eve::msg::Request m_request;

  /// This will ready the resquest and parse it. The request might not have be
  /// fully read in one go. The function will call itself when necessary to
  /// continuing reading. Because of this it is important to keep the state in
  /// the parser.
  void read();

  /// Once a reply has been written this will write it to the client.
  void write(const eve::msg::Reply& reply);
};

}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_CONNECTION_HPP