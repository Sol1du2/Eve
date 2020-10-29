#ifndef EVE_MSG_HEADER_H
#define EVE_MSG_HEADER_H

#include <sstream>
#include <string>

namespace eve
{
namespace msg
{
struct Header
{
  std::string name;
  std::string value;

  std::string to_string() const
  {
    std::stringstream stream;
    stream << "Header: " << name << " - " << value << "\n";
    return stream.str();
  }
};
}  // namespace msg
}  // namespace eve

#endif  // EVE_MSG_HEADER_H