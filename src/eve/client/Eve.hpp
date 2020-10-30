#ifndef EVE_CLIENT_EVE_HPP
#define EVE_CLIENT_EVE_HPP

#include <boost/asio.hpp>
#include <set>

#include "../msg/Reply.hpp"
#include "../msg/Request.hpp"

namespace eve
{
namespace client
{
/// This class is the main class for the Eve-client. It supports the following
/// requests:
/// - Get an e-mail
/// - Get an e-mail status
/// - Save an e-mail to the server (and send it to the smtp server)
/// - Delete an e-mail
/// The client blocks until it receives a reply from the server (or it
/// times-out).
class Eve
{
 public:
  Eve(const std::string& address, const std::string& port);

  Eve(const Eve&) = delete;

  Eve& operator=(const Eve&) = delete;

  std::string send_email(const std::string& to_email,
                         const std::string& from_email,
                         const std::string& subject, const std::string& body);

  bool delete_email(const std::string& id);

  std::string get_email(const std::string& id);

  std::string get_email_status(const std::string& id);

 private:
  boost::asio::io_context m_io_context;
  boost::asio::ip::tcp::socket m_socket;
  std::string m_address;
  std::string m_port;

  void connect();

  void close();

  eve::msg::Reply send_request(const eve::msg::Request& request);

  void read(std::array<char, 8192>& buffer, eve::msg::Reply& reply);
};

}  // namespace client
}  // namespace eve

#endif  // EVE_CLIENT_EVE_HPP