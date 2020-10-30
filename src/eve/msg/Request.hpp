#ifndef EVE_MSG_REQUEST_HPP
#define EVE_MSG_REQUEST_HPP

#include <boost/asio/buffer.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "Header.hpp"

namespace eve
{
namespace msg
{
/// A request received from a client.
struct Request
{
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  int content_length;
  std::vector<Header> headers;

  std::string content;

  /// Convert the reply into a vector of buffers. The buffers do not own the
  /// underlying memory blocks, therefore the reply object must remain valid and
  /// not be changed until the write operation has completed.
  std::vector<boost::asio::const_buffer> to_buffers() const;

  std::string to_string() const;
};

}  // namespace msg
}  // namespace eve

#endif  // EVE_MSG_REQUEST_HPP
