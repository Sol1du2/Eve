#include "Util.hpp"

namespace eve
{
namespace util
{
std::vector<std::string> Util::split_string(const std::string& delimiter,
                                            std::string string)
{
  std::size_t pos = 0;
  std::vector<std::string> split_string;
  while ((pos = string.find(delimiter)) != std::string::npos)
  {
    std::string token = string.substr(0, pos);
    split_string.push_back(token);
    string.erase(0, pos + delimiter.length());
  }
  split_string.push_back(string);

  return split_string;
}

}  // namespace util
}  // namespace eve
