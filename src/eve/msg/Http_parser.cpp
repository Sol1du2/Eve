#include "Http_parser.hpp"

namespace eve
{
namespace msg
{
Http_parser::Http_parser() { set_parse_request(); }

void Http_parser::set_parse_request()
{
  m_state = Http_parser::State::Method_start;
  m_content_read = 0;
  m_current_consumption_string = "";
}

void Http_parser::set_parse_reply()
{
  m_state = Http_parser::State::Http_version_h;
  m_content_read = 0;
  m_current_consumption_string = "";
}

Http_parser::Parse_result Http_parser::consume(char input, Request& request)
{
  switch (m_state)
  {
    case Http_parser::State::Method_start:
      if (!is_char(input) || is_ctl(input) || is_tspecial(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        m_state = Http_parser::State::Method;
        request.method.push_back(input);
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Method:
      if (input == ' ')
      {
        m_state = Http_parser::State::Uri;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        request.method.push_back(input);
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Uri:
      if (input == ' ')
      {
        m_state = Http_parser::State::Http_version_h;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (is_ctl(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        request.uri.push_back(input);
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Http_version_h:
      if (input == 'H')
      {
        m_state = Http_parser::State::Http_version_t_1;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_t_1:
      if (input == 'T')
      {
        m_state = Http_parser::State::Http_version_t_2;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_t_2:
      if (input == 'T')
      {
        m_state = Http_parser::State::Http_version_p;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_p:
      if (input == 'P')
      {
        m_state = Http_parser::State::Http_version_slash;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_slash:
      if (input == '/')
      {
        request.http_version_major = 0;
        request.http_version_minor = 0;
        m_state = Http_parser::State::Http_version_major_start;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_major_start:
      if (is_digit(input))
      {
        request.http_version_major =
            request.http_version_major * 10 + input - '0';
        m_state = Http_parser::State::Http_version_major;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_major:
      if (input == '.')
      {
        m_state = Http_parser::State::Http_version_minor_start;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (is_digit(input))
      {
        request.http_version_major =
            request.http_version_major * 10 + input - '0';
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_minor_start:
      if (is_digit(input))
      {
        request.http_version_minor =
            request.http_version_minor * 10 + input - '0';
        m_state = Http_parser::State::Http_version_minor;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_minor:
      if (input == '\r')
      {
        m_state = Http_parser::State::Expecting_newline_1;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (is_digit(input))
      {
        request.http_version_minor =
            request.http_version_minor * 10 + input - '0';
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Expecting_newline_1:
      if (input == '\n')
      {
        m_state = Http_parser::State::Header_line_start;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Header_line_start:
      if (input == '\r')
      {
        m_state = Http_parser::State::Expecting_newline_3;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (!request.headers.empty() && (input == ' ' || input == '\t'))
      {
        m_state = Http_parser::State::Header_lws;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        request.headers.push_back(Header());
        request.headers.back().name.push_back(input);
        m_state = Http_parser::State::Header_name;
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Header_lws:
      if (input == '\r')
      {
        m_state = Http_parser::State::Expecting_newline_2;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (input == ' ' || input == '\t')
      {
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (is_ctl(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        m_state = Http_parser::State::Header_value;
        request.headers.back().value.push_back(input);
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Header_name:
      if (input == ':')
      {
        m_state = Http_parser::State::Space_before_header_value;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        request.headers.back().name.push_back(input);
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Space_before_header_value:
      if (input == ' ')
      {
        m_state = Http_parser::State::Header_value;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Header_value:
      if (input == '\r')
      {
        if (request.headers.back().name == "Content-Length")
        {
          try
          {
            request.content_length = std::stoi(request.headers.back().value);
          }
          catch (const std::exception&)
          {
            return Http_parser::Parse_result::Error;
          }
        }

        m_state = Http_parser::State::Expecting_newline_2;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (is_ctl(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        request.headers.back().value.push_back(input);
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Expecting_newline_2:
      if (input == '\n')
      {
        m_state = Http_parser::State::Header_line_start;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Expecting_newline_3:
      if (input == '\n')
      {
        if (request.content_length == 0)
        {
          return Http_parser::Parse_result::Successful;
        }
        m_state = Http_parser::State::Body;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Body:
      request.content.push_back(input);
      ++m_content_read;
      if (m_content_read == request.content_length)
      {
        return Http_parser::Parse_result::Successful;
      }
      return Http_parser::Parse_result::Indeterminate;
    default:
      return Http_parser::Parse_result::Error;
  }
}

Http_parser::Parse_result Http_parser::consume(char input, Reply& reply)
{
  switch (m_state)
  {
    case Http_parser::State::Http_version_h:
      if (input == 'H')
      {
        m_state = Http_parser::State::Http_version_t_1;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_t_1:
      if (input == 'T')
      {
        m_state = Http_parser::State::Http_version_t_2;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_t_2:
      if (input == 'T')
      {
        m_state = Http_parser::State::Http_version_p;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_p:
      if (input == 'P')
      {
        m_state = Http_parser::State::Http_version_slash;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_slash:
      if (input == '/')
      {
        m_state = Http_parser::State::Http_version_major_start;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_major_start:
      if (is_digit(input))
      {
        m_state = Http_parser::State::Http_version_major;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_major:
      if (input == '.')
      {
        m_state = Http_parser::State::Http_version_minor_start;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (is_digit(input))
      {
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_minor_start:
      if (is_digit(input))
      {
        m_state = Http_parser::State::Http_version_minor;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Http_version_minor:
      if (input == ' ')
      {
        m_state = Http_parser::State::Status_digit_1;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (is_digit(input))
      {
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Status_digit_1:
    {
      if (is_digit(input))
      {
        m_current_consumption_string += input;
        m_state = Http_parser::State::Status_digit_2;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    }
    case Http_parser::State::Status_digit_2:
    {
      if (is_digit(input))
      {
        m_current_consumption_string += input;
        m_state = Http_parser::State::Status_digit_3;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    }
    case Http_parser::State::Status_digit_3:
    {
      if (is_digit(input))
      {
        m_current_consumption_string += input;
        m_state = Http_parser::State::Status_digit_3;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (input == ' ')
      {
        m_state = Http_parser::State::Status_msg;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    }
    case Http_parser::State::Status_msg:
    {
      if (input == '\r')
      {
        m_state = Http_parser::State::Expecting_newline_1;
        return Http_parser::Parse_result::Indeterminate;
      }
      if (is_char(input))
      {
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    }
    case Http_parser::State::Expecting_newline_1:
      if (input == '\n')
      {
        m_state = Http_parser::State::Header_line_start;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Header_line_start:
      if (input == '\r')
      {
        m_state = Http_parser::State::Expecting_newline_3;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (!reply.headers.empty() && (input == ' ' || input == '\t'))
      {
        m_state = Http_parser::State::Header_lws;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        reply.headers.push_back(Header());
        reply.headers.back().name.push_back(input);
        m_state = Http_parser::State::Header_name;
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Header_lws:
      if (input == '\r')
      {
        m_state = Http_parser::State::Expecting_newline_2;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (input == ' ' || input == '\t')
      {
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (is_ctl(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        m_state = Http_parser::State::Header_value;
        reply.headers.back().value.push_back(input);
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Header_name:
      if (input == ':')
      {
        m_state = Http_parser::State::Space_before_header_value;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        reply.headers.back().name.push_back(input);
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Space_before_header_value:
      if (input == ' ')
      {
        m_state = Http_parser::State::Header_value;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Header_value:
      if (input == '\r')
      {
        if (reply.headers.back().name == "Content-Length")
        {
          try
          {
            reply.content_length = std::stoi(reply.headers.back().value);
          }
          catch (const std::exception&)
          {
            return Http_parser::Parse_result::Error;
          }
        }

        m_state = Http_parser::State::Expecting_newline_2;
        return Http_parser::Parse_result::Indeterminate;
      }
      else if (is_ctl(input))
      {
        return Http_parser::Parse_result::Error;
      }
      else
      {
        reply.headers.back().value.push_back(input);
        return Http_parser::Parse_result::Indeterminate;
      }
    case Http_parser::State::Expecting_newline_2:
      if (input == '\n')
      {
        m_state = Http_parser::State::Header_line_start;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Expecting_newline_3:
      if (input == '\n')
      {
        if (reply.content_length == 0)
        {
          return Http_parser::Parse_result::Successful;
        }
        m_state = Http_parser::State::Body;
        return Http_parser::Parse_result::Indeterminate;
      }
      else
      {
        return Http_parser::Parse_result::Error;
      }
    case Http_parser::State::Body:
      reply.content.push_back(input);
      ++m_content_read;
      if (m_content_read == reply.content_length)
      {
        return Http_parser::Parse_result::Successful;
      }
      return Http_parser::Parse_result::Indeterminate;
    default:
      return Http_parser::Parse_result::Error;
  }
}

bool Http_parser::is_char(int c) { return c >= 0 && c <= 127; }

bool Http_parser::is_ctl(int c) { return (c >= 0 && c <= 31) || (c == 127); }

bool Http_parser::is_tspecial(int c)
{
  switch (c)
  {
    case '(':
    case ')':
    case '<':
    case '>':
    case '@':
    case ',':
    case ';':
    case ':':
    case '\\':
    case '"':
    case '/':
    case '[':
    case ']':
    case '?':
    case '=':
    case '{':
    case '}':
    case ' ':
    case '\t':
      return true;
    default:
      return false;
  }
}

bool Http_parser::is_digit(int c) { return c >= '0' && c <= '9'; }

}  // namespace msg
}  // namespace eve
