#ifndef EVE_SERVER_API_INDEX_HANDLER_HPP
#define EVE_SERVER_API_INDEX_HANDLER_HPP

#include "../../msg/Reply.hpp"
#include "../../msg/Request.hpp"

namespace eve
{
namespace server
{
namespace api
{
/// Simple handler to show the index page. uri -> /
class Index_handler
{
 public:
  static std::string get_handler_uri();

  Index_handler();

  eve::msg::Reply handle(const eve::msg::Request& request,
                                 const std::string& args) const;
};

}  // namespace api
}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_API_INDEX_HANDLER_HPP