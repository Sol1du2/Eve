#ifndef EVE_MSG_REPLY_HPP
#define EVE_MSG_REPLY_HPP

#include <boost/asio.hpp>
#include <string>
#include <vector>

#include "Header.hpp"

namespace eve
{
namespace msg
{

/// This struct represents a reply from the server.
struct Reply
{
  /// The status of the reply.
  enum class Status_type
  {
    Ok = 200,
    Not_found = 404,
    Bad_request = 400,
    Internal_server_error = 500,
    Not_implemented = 501
  };

  std::vector<Header> headers;

  std::string content;

  Status_type status;

  /// Convert the reply into a vector of buffers. The buffers do not own the
  /// underlying memory blocks, therefore the reply object must remain valid and
  /// not be changed until the write operation has completed.
  std::vector<boost::asio::const_buffer> to_buffers() const;

  /// Get a stock reply of a specific status.
  static Reply stock_reply(Status_type status);
};

}  // namespace msg
}  // namespace eve

#endif  // EVE_MSG_REPLY_HPP
