#include "Index_handler.hpp"

namespace eve
{
namespace server
{
namespace api
{
namespace
{
constexpr const char* const HANDLER_URI = "/";
}

std::string Index_handler::get_handler_uri() { return HANDLER_URI; }

Index_handler::Index_handler() {}

eve::msg::Reply Index_handler::handle(
    const eve::msg::Request& /*request*/,
    const std::string& /*args*/) const
{
  eve::msg::Reply reply;
  reply.status = eve::msg::Reply::Status_type::Ok;
  reply.content =
      "<html>"
      "<head><title>Welcome Page Eevee</title></head>"
      "<body>"
      "<h1>Welcome to Eve's start-up screen!!!</h1>"
      "<div>"
      "<p>This is a small HTTP server that sends e-mails to an SMTP "
      "server.</p>"
      "<p>To send an e-mail use the json API via the following URI: <a "
      "href=\"/jsonApi\">/jsonApi</a></p>"
      "<ul>"
      "<li>A GET will return all e-mails, along with their status.</li>"
      "<li>A GET with the following arg: ?id=&lt;email_id&gt; will return the "
      "e-mail with that id.</li>"
      "<li>A GET with the following args: ?id=&LT;email_id&gt;&status_only "
      "will return the status of that e-mail with that id.</li>"
      "<li>A POST with json content will send an e-mail. The json must contain "
      "&LT;from_email&gt;, &LT;to_email&gt;, &LT;subject&gt; and "
      "&LT;body&gt; This will prompt the server to send the e-mail to the SMTP "
      "server. It will attempt to send the e-mail 5 times.</li>"
      "<li>A DELETE with the following arg: ?id=&LT;email_id&gt; will delete "
      "that e-mail.</li>"
      "</ul>"
      "<p>An email can have the following status:</p>"
      "<ul>"
      "<li>Sending - The e-mail is being sent.</li>"
      "<li>Sent - The e-mail is being sent.</li>"
      "<li>Failed - The e-mail failed to send. The server will try again.</li>"
      "<li>Gave up - The server has unsuccessfully attempted to send the "
      "e-mail 5 times and gave up.</li>"
      "</ul>"
      "</div>"
      "</body>"
      "</html>";

  reply.headers.resize(2);
  reply.headers[0].name = "Content-Length";
  reply.headers[0].value = std::to_string(reply.content.size());
  reply.headers[1].name = "Content-Type";
  reply.headers[1].value = "text/html";

  return reply;
}

}  // namespace api
}  // namespace server
}  // namespace eve