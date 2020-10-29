#include "Data_definition.hpp"

namespace eve
{
namespace server
{
namespace data
{
std::string Data_definition::to_string(const Email_status& status)
{
  switch (status)
  {
    case Email_status::Sending:
      return "Sending";
    case Email_status::Sent:
      return "Sent";
    case Email_status::Failed:
      return "Failed";
    case Email_status::Gave_up:
      return "Gave up";
    default:
      return "Undefined";
  }
}
}  // namespace data
}  // namespace server
}  // namespace eve