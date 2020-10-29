#ifndef EVE_SERVER_API_JSON_HANDLER_HPP
#define EVE_SERVER_API_JSON_HANDLER_HPP

#include <boost/uuid/uuid.hpp>

#include "../../msg/Reply.hpp"
#include "../../msg/Request.hpp"

namespace eve
{
namespace server
{
namespace api
{
/// This handler handles all requests to the jsonApi.
/// Currently it supports get, post and delete.
/// - A get request with no arguments returns all e-mails stored on the server.
/// - A get request with an email id returns that e-mail, if found.
/// - A get request with an email id and the status_only keyword returns that
/// e-mail's status, if found.
/// - A post request stores a new e-mail in the server and sends it to the smtp
/// server. The post data must be formatted in json with the following keys:
/// to_email, from_email, subject, body. After the e-mail is saved a uuid is
/// returned to the client. This uuid is also visible with a get request.
/// - A delete request must contain the id of an e-mail and will remove it from
/// storage, if found.
class Json_handler
{
 public:
  static std::string get_handler_uri();

  Json_handler();

  eve::msg::Reply handle(const eve::msg::Request& request,
                         const std::string& args) const;

 private:
  eve::msg::Reply handle_get(const std::string& args) const;
  eve::msg::Reply handle_post(const std::string& request_body) const;
  eve::msg::Reply handle_delete(const std::string& args) const;

  eve::msg::Reply ok_reply(const std::string& content) const;

  void send_smtp_email(const std::string& from_email,
                       const std::string& to_email, const std::string& subject,
                       const std::string& body,
                       const boost::uuids::uuid& email_uuid) const;
};

}  // namespace api
}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_API_JSON_HANDLER_HPP