#ifndef EVE_MSG_REQUEST_HANDLER_HPP
#define EVE_MSG_REQUEST_HANDLER_HPP

#include <map>
#include <variant>

#include "../../msg/Reply.hpp"
#include "../../msg/Request.hpp"
#include "Index_handler.hpp"
#include "Json_handler.hpp"

namespace eve
{
namespace server
{
namespace api
{
/// This is the generic request to handle all requests. To add a new request
/// type it must be added to the Handler_types definition and register it in the
/// register_handlers function. A request handler class must implement a handle
/// function that receives a request and returns a reply.
class Request_handler
{
 public:
  Request_handler();

  eve::msg::Reply handle(const eve::msg::Request& request) const;

 private:
  /// All the available handle types. Add a type to the variant for new
  /// handlers.
  using Handler_types = std::variant<Json_handler, Index_handler>;

  eve::msg::Request m_request;
  /// A map of all handlers. The key should be a string of the uri that
  /// corresponds to the handler. The value is a function that creates the right
  /// handler.
  std::map<std::string, std::function<Handler_types()>> m_handlers_map;

  template <class T>
  Handler_types create_handler_instance() const
  {
    return Handler_types(T());
  }

  /// This function is responsible for registering the handlers in the map.
  void register_handlers();
};

}  // namespace api
}  // namespace server
}  // namespace eve

#endif  // EVE_MSG_REQUEST_HANDLER_HPP