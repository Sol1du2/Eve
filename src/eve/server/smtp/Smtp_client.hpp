#ifndef EVE_SERVER_SMTP_SMTP_CLIENT_HPP
#define EVE_SERVER_SMTP_SMTP_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <string>

namespace eve
{
namespace server
{
namespace smtp
{
/// This class takes care of connecting to the smtp server and sending out the
/// e-mail. The simplest version of the SMTP protocol is being used. That means
/// commands like EHLO and authentication are not supported.
/// The supported commands can be seen wikipedia's example here:
/// https://en.wikipedia.org/wiki/Simple_Mail_Transfer_Protocol#SMTP_transport_example
class Smtp_client
{
 public:
  Smtp_client(const std::string& client_address, const std::string& address,
              const std::string& port);

  Smtp_client(const Smtp_client&) = delete;
  Smtp_client& operator=(const Smtp_client&) = delete;

  /// This function will block until the message is sent or an exception is
  /// thrown.
  /// @throw Smtp_code_exception
  /// @throw std::exception
  void send_message(const std::string& from_email, const std::string& to_email,
                    const std::string& subject, const std::string& body);

 private:
  boost::asio::io_context m_io_context;
  boost::asio::ip::tcp::socket m_socket;
  std::string m_client_address;
  std::string m_smtp_address;
  std::string m_smtp_port;

  /// Reads a message from the smtp server and checks the expected code.
  /// @throw Smtp_code_exception in case the code is not what was expected.
  void read_smtp_message(const std::string& expected_code);

  /// Sends a message and checks the reply for the expected code if check_reply
  /// is true.
  void send_smtp_message(const std::string& message, bool check_reply,
                         const std::string& expected_code);

  /// Sends a message but does not check the reply
  void send_smtp_message(const std::string& message);
};
}  // namespace smtp
}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_SMTP_SMTP_CLIENT_HPP