#include "Request_handler.hpp"

#include <vector>

#include "../../util/Util.hpp"

namespace eve
{
namespace server
{
namespace api
{
Request_handler::Request_handler() { register_handlers(); }

eve::msg::Reply Request_handler::handle(
    const eve::msg::Request& request) const
{
  const auto split_uri = eve::util::Util::split_string("?", request.uri);
  const auto it = m_handlers_map.find(split_uri.front());
  if (it != m_handlers_map.end())
  {
    return std::visit(
        [&](auto&& handler) {
          return handler.handle(request,
                                split_uri.size() > 1u ? split_uri.back() : "");
        },
        it->second());
  }

  return eve::msg::Reply::stock_reply(
      eve::msg::Reply::Status_type::Not_found);
}

void Request_handler::register_handlers()
{
  std::function<Handler_types()> indexFunction =
      std::bind(&Request_handler::create_handler_instance<Index_handler>, this);
  m_handlers_map.insert_or_assign(Index_handler::get_handler_uri(),
                                  indexFunction);

  std::function<Handler_types()> jsonApiFunction =
      std::bind(&Request_handler::create_handler_instance<Json_handler>, this);
  m_handlers_map.insert_or_assign(Json_handler::get_handler_uri(),
                                  jsonApiFunction);
}

}  // namespace api
}  // namespace server
}  // namespace eve