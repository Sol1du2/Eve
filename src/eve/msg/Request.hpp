#ifndef EVE_MSG_REQUEST_HPP
#define EVE_MSG_REQUEST_HPP

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

  std::string body;

  std::string to_string() const
  {
    std::stringstream stream;
    for (const auto& header : headers)
    {
      stream << header.to_string();
    }
    stream << "Version: " << http_version_major << "." << http_version_minor
           << "\nMethod: " << method << "\nUri: " << uri << "\nBody:\n"
           << body << "\n";
    return stream.str();
  }
};

}  // namespace msg
}  // namespace eve

#endif  // EVE_MSG_REQUEST_HPP
