#include "Reply.hpp"

#include <string>

namespace eve
{
namespace msg
{
namespace
{
const std::string OK_STRING = "HTTP/1.0 200 OK\r\n";
const std::string NOT_FOUND_STRING = "HTTP/1.0 404 Not Found\r\n";
const std::string BAD_REQUEST_STRING = "HTTP/1.0 400 Bad Request\r\n";
const std::string INTERNAL_SERVER_ERROR_STRING =
    "HTTP/1.0 500 Internal Server Error\r\n";
const std::string NOT_IMPLEMENTED_STRING = "HTTP/1.0 501 Not Implemented\r\n";

constexpr char NAME_VALUE_SEPARATOR[] = {':', ' '};
constexpr char CRLF[] = {'\r', '\n'};

constexpr char OK_STOCK[] = "";
constexpr char NOT_FOUND_STOCK[] =
    "<html>"
    "<head><title>Not Found</title></head>"
    "<body><h1>404 Not Found</h1></body>"
    "</html>";
constexpr char BAD_REQUEST_STOCK[] =
    "<html>"
    "<head><title>Bad Request</title></head>"
    "<body><h1>400 Bad Request</h1></body>"
    "</html>";
constexpr char INTERNAL_SERVER_ERROR_STOCK[] =
    "<html>"
    "<head><title>Internal Server Error</title></head>"
    "<body><h1>500 Internal Server Error</h1></body>"
    "</html>";
constexpr char NOT_IMPLEMENTED_STOCK[] =
    "<html>"
    "<head><title>Not Implemented</title></head>"
    "<body><h1>501 Not Implemented</h1></body>"
    "</html>";

boost::asio::const_buffer to_buffer(Reply::Status_type status)
{
  switch (status)
  {
    case Reply::Status_type::Ok:
      return boost::asio::buffer(OK_STRING);
    case Reply::Status_type::Not_found:
      return boost::asio::buffer(NOT_FOUND_STRING);
    case Reply::Status_type::Bad_request:
      return boost::asio::buffer(BAD_REQUEST_STRING);
    case Reply::Status_type::Internal_server_error:
      return boost::asio::buffer(INTERNAL_SERVER_ERROR_STRING);
    case Reply::Status_type::Not_implemented:
      return boost::asio::buffer(NOT_IMPLEMENTED_STRING);
    default:
      return boost::asio::buffer(INTERNAL_SERVER_ERROR_STRING);
  }
}

std::string to_string(Reply::Status_type status)
{
  switch (status)
  {
    case Reply::Status_type::Ok:
      return OK_STOCK;
    case Reply::Status_type::Not_found:
      return NOT_FOUND_STOCK;
    case Reply::Status_type::Bad_request:
      return BAD_REQUEST_STOCK;
    case Reply::Status_type::Internal_server_error:
      return INTERNAL_SERVER_ERROR_STOCK;
    case Reply::Status_type::Not_implemented:
      return NOT_IMPLEMENTED_STOCK;
    default:
      return INTERNAL_SERVER_ERROR_STOCK;
  }
}

}  // namespace

std::vector<boost::asio::const_buffer> Reply::to_buffers() const
{
  std::vector<boost::asio::const_buffer> buffers;
  buffers.push_back(to_buffer(status));
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

Reply Reply::stock_reply(Reply::Status_type status)
{
  Reply reply;
  reply.status = status;
  reply.content = to_string(status);
  reply.headers.resize(2);
  reply.headers[0].name = "Content-Length";
  reply.headers[0].value = std::to_string(reply.content.size());
  reply.headers[1].name = "Content-Type";
  reply.headers[1].value = "text/html";
  return reply;
}

}  // namespace msg
}  // namespace eve
