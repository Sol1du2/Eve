#ifndef EVE_SERVER_SERVER_HPP
#define EVE_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <set>

#include "Connection.hpp"

namespace eve
{
namespace server
{
/// This is the main class for the server. The server receives HTTP requests on
/// the jsonApi uri. Any data should be json formatted and the responses are
/// also json. When email information is sent to the server, the server will
/// attempt to send it to its smtp server. This is done in a separate thread.
/// The server will attempt to send the e-mail 5 times.
///
/// The server handles async connections in a single thread.
///
class Eve
{
 public:
  Eve(const std::string& address, const std::string& port,
      const std::string& smtp_address, const std::string& smtp_port);

  Eve(const Eve&) = delete;

  Eve& operator=(const Eve&) = delete;

  /// This function starts the server. It will block until the server receives a
  /// SIGINT SIGTERM or SIGQUIT.
  void run();

 private:
  boost::asio::io_context m_io_context;
  boost::asio::ip::tcp::acceptor m_tcp_acceptor;
  boost::asio::signal_set m_signals;

  /// This set holds all current, ongoing connections to the server.
  std::set<std::shared_ptr<Connection>> m_connections;

  void accept_connections();

  void wait_signals();

  void close_all_connections();
};

}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_SERVER_HPP