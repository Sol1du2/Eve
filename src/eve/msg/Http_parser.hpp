#ifndef EVE_MSG_PARSER_HPP
#define EVE_MSG_PARSER_HPP

#include "Request.hpp"

namespace eve
{
namespace msg
{
/// Http Parser for incoming requests.
class Http_parser
{
 public:
  /// Construct ready to parse the request method.
  Http_parser();

  /// Reset to initial parser state.
  void reset();

  /// Result of parse.
  enum class Parse_result
  {
    Successful = 0,
    Error = 1,
    Indeterminate = 2
  };

  /// Parse some data. The enum return value is Successful when a complete
  /// request has been parsed, Error if the data is invalid, Indeterminate when
  /// more data is required. The Input_iterator return value indicates how much
  /// of the input has been consumed.
  template <class Input_iterator>
  Parse_result parse(Input_iterator begin, Input_iterator end, Request& request)
  {
    if (begin == end && m_state == State::Body)
    {
      return Parse_result::Successful;
    }

    while (begin != end)
    {
      Parse_result result = consume(*begin++, request);
      if (result == Parse_result::Successful || result == Parse_result::Error)
      {
        return result;
      }
    }

    return Parse_result::Indeterminate;
  }

 private:
  /// The current state of the parser.
  enum class State
  {
    Method_start = 0,
    Method = 1,
    Uri = 2,
    Http_version_h = 3,
    Http_version_t_1 = 4,
    Http_version_t_2 = 5,
    Http_version_p = 6,
    Http_version_slash = 7,
    Http_version_major_start = 8,
    Http_version_major = 9,
    Http_version_minor_start = 10,
    Http_version_minor = 11,
    Expecting_newline_1 = 12,
    Header_line_start = 13,
    Header_lws = 14,
    Header_name = 15,
    Space_before_header_value = 16,
    Header_value = 17,
    Expecting_newline_2 = 18,
    Expecting_newline_3 = 19,
    Body = 20
  };

  State m_state;

  int m_content_read;

  /// Handle the next character of input.
  Parse_result consume(char input, Request& request);

  /// Check if a byte is an HTTP character.
  static bool is_char(int c);

  /// Check if a byte is an HTTP control character.
  static bool is_ctl(int c);

  /// Check if a byte is defined as an HTTP tspecial character.
  static bool is_tspecial(int c);

  /// Check if a byte is a digit.
  static bool is_digit(int c);
};

}  // namespace msg
}  // namespace eve

#endif  // EVE_MSG_PARSER_HPP
