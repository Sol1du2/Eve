#include "Request.hpp"

namespace eve
{
namespace msg
{
namespace
{
constexpr char NAME_VALUE_SEPARATOR[] = {':', ' '};
constexpr char VALUE_SEPARATOR[] = {' '};
constexpr char VERSION_DOT[] = {'.'};
constexpr char CRLF[] = {'\r', '\n'};
constexpr char VERSION_START[] = {'H', 'T', 'T', 'P', '/'};
}  // namespace
std::vector<boost::asio::const_buffer> Request::to_buffers() const
{
  std::vector<boost::asio::const_buffer> buffers;

  buffers.push_back(boost::asio::buffer(method));
  buffers.push_back(boost::asio::buffer(VALUE_SEPARATOR));

  buffers.push_back(boost::asio::buffer(uri));
  buffers.push_back(boost::asio::buffer(VALUE_SEPARATOR));
  
  buffers.push_back(boost::asio::buffer(VERSION_START));
  std::string major = std::to_string(http_version_major);
  std::string minor = std::to_string(http_version_minor);
  buffers.push_back(boost::asio::buffer(major));
  buffers.push_back(boost::asio::buffer(VERSION_DOT));
  buffers.push_back(boost::asio::buffer(minor));
  buffers.push_back(boost::asio::buffer(CRLF));

  for (const auto& header : headers)
  {
    buffers.push_back(boost::asio::buffer(header.name));
    buffers.push_back(boost::asio::buffer(NAME_VALUE_SEPARATOR));
    buffers.push_back(boost::asio::buffer(header.value));
    buffers.push_back(boost::asio::buffer(CRLF));
  }

  buffers.push_back(boost::asio::buffer(CRLF));
  buffers.push_back(boost::asio::buffer(content));
  return buffers;
}

std::string Request::to_string() const
{
  std::stringstream stream;
  for (const auto& header : headers)
  {
    stream << header.to_string();
  }
  stream << "Version: " << http_version_major << "." << http_version_minor
         << "\nMethod: " << method << "\nUri: " << uri << "\nBody:\n"
         << content << "\n";
  return stream.str();
}
}  // namespace msg
}  // namespace eve