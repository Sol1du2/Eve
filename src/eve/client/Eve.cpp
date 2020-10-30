#include "Eve.hpp"

#include <array>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../msg/Http_parser.hpp"
#include "../msg/Request.hpp"
#include "../util/Eve_exception.hpp"

namespace eve
{
namespace client
{
Eve::Eve(const std::string& address, const std::string& port)
    : m_io_context(1), m_socket(m_io_context), m_address(address), m_port(port)
{
}

std::string Eve::send_email(const std::string& to_email,
                            const std::string& from_email,
                            const std::string& subject, const std::string& body)
{
  connect();

  try
  {
    eve::msg::Request request;
    request.method = "POST";
    request.uri = "/jsonApi";
    request.http_version_major = 1;
    request.http_version_minor = 1;

    nlohmann::json json_body;
    json_body["to_email"] = to_email;
    json_body["from_email"] = from_email;
    json_body["subject"] = subject;
    json_body["body"] = body;

    std::stringstream stream;
    stream << json_body;
    request.content = stream.str();
    request.content_length = request.content.size();

    request.headers.resize(2);
    request.headers[0].name = "Content-Length";
    request.headers[0].value = std::to_string(request.content_length);
    request.headers[1].name = "Content-Type";
    request.headers[1].value = "application/json";

    eve::msg::Reply reply = send_request(request);
    nlohmann::json json_content = nlohmann::json::parse(reply.content);

    close();
    return json_content["id"];
  }
  catch (const std::exception& exception)
  {
    close();
    throw exception;
  }
}

bool Eve::delete_email(const std::string& id)
{
  connect();

  try
  {
    eve::msg::Request request;
    request.method = "DELETE";
    request.uri = "/jsonApi?id=" + id;
    request.http_version_major = 1;
    request.http_version_minor = 1;
    request.content_length = 0;

    request.headers.resize(2);
    request.headers[0].name = "Content-Length";
    request.headers[0].value = std::to_string(request.content_length);
    request.headers[1].name = "Content-Type";
    request.headers[1].value = "application/json";

    eve::msg::Reply reply = send_request(request);
    nlohmann::json json_content = nlohmann::json::parse(reply.content);

    close();
    return json_content.contains("success") ? true : false;
  }
  catch (const std::exception& exception)
  {
    close();
    throw exception;
  }
}

std::string Eve::get_email(const std::string& id)
{
  connect();

  try
  {
    eve::msg::Request request;
    request.method = "GET";
    request.uri = "/jsonApi?id=" + id;
    request.http_version_major = 1;
    request.http_version_minor = 1;
    request.content_length = 0;

    request.headers.resize(2);
    request.headers[0].name = "Content-Length";
    request.headers[0].value = std::to_string(request.content_length);
    request.headers[1].name = "Content-Type";
    request.headers[1].value = "application/json";

    eve::msg::Reply reply = send_request(request);
    nlohmann::json json_content = nlohmann::json::parse(reply.content);

    close();

    const auto error_it = json_content.find("error");
    if (error_it == json_content.end())
    {
      std::stringstream stream;
      stream << json_content;
      return stream.str();
    }
    return (*error_it)["message"];
  }
  catch (const std::exception& exception)
  {
    close();
    throw exception;
  }
}

std::string Eve::get_email_status(const std::string& id)
{
  connect();

  try
  {
    eve::msg::Request request;
    request.method = "GET";
    request.uri = "/jsonApi?id=" + id + "&status_only";
    request.http_version_major = 1;
    request.http_version_minor = 1;
    request.content_length = 0;

    request.headers.resize(2);
    request.headers[0].name = "Content-Length";
    request.headers[0].value = std::to_string(request.content_length);
    request.headers[1].name = "Content-Type";
    request.headers[1].value = "application/json";

    eve::msg::Reply reply = send_request(request);
    nlohmann::json json_content = nlohmann::json::parse(reply.content);

    close();
    const auto status_it = json_content.find("error");
    if (status_it == json_content.end())
    {
      return json_content["status"];
    }
    return (*status_it)["message"];
  }
  catch (const std::exception& exception)
  {
    close();
    throw exception;
  }
}

void Eve::connect()
{
  boost::asio::ip::tcp::resolver resolver(m_io_context);
  boost::asio::ip::tcp::resolver::query query(m_address, m_port);
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
      resolver.resolve(query);
  boost::asio::connect(m_socket, endpoint_iterator);
}

void Eve::close()
{
  boost::system::error_code ignored_ec;
  if (m_socket.is_open())
  {
    m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    m_socket.close(ignored_ec);
  }
}

eve::msg::Reply Eve::send_request(const eve::msg::Request& request)
{
  boost::asio::write(m_socket, request.to_buffers());

  std::array<char, 8192> buffer;
  eve::msg::Reply reply;
  read(buffer, reply);

  return reply;
}

void Eve::read(std::array<char, 8192>& buffer, eve::msg::Reply& reply)
{
  std::size_t bytes_read = m_socket.read_some(boost::asio::buffer(buffer));
  eve::msg::Http_parser parser;
  parser.set_parse_reply();
  const auto parse_result =
      parser.parse(buffer.data(), buffer.data() + bytes_read, reply);

  switch (parse_result)
  {
    case eve::msg::Http_parser::Parse_result::Successful:
      // Do nothing.
      break;
    case eve::msg::Http_parser::Parse_result::Error:
      throw eve::util::Eve_exception("Failed to parse reply.");
      break;
    case eve::msg::Http_parser::Parse_result::Indeterminate:
      read(buffer, reply);
      break;
    default:
      throw eve::util::Eve_exception("Unknown error while parsing.");
  }
}

}  // namespace client
}  // namespace eve