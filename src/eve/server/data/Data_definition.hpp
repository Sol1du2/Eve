#ifndef EVE_SERVER_DATA_DATA_DEFINITION_HPP
#define EVE_SERVER_DATA_DATA_DEFINITION_HPP

#include <boost/uuid/uuid.hpp>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>

namespace eve
{
namespace server
{
namespace data
{
/// This represents the status of an e-mail.
class Data_definition
{
 public:
  enum class Email_status
  {
    /// The e-mail is being sent.
    Sending = 0,
    /// The e-mail has been sent.
    Sent = 1,
    /// Sending the e-mail failed and the server will attempt again.
    Failed = 2,
    /// Sending the e-mail has failed more than the maximum amount of times and
    /// the server has given up.
    Gave_up = 3,
  };

  /// Converts the e-mail status enum to a string.
  static std::string to_string(const Email_status& status);
};

}  // namespace data
}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_DATA_DATA_DEFINITION_HPP