#ifndef EVE_UTIL_UTIL_HPP
#define EVE_UTIL_UTIL_HPP

#include <string>
#include <vector>

namespace eve
{
namespace util
{
class Util
{
 public:
  static std::vector<std::string> split_string(const std::string& delimiter,
                                               std::string string);
};

}  // namespace util
}  // namespace eve

#endif  // EVE_UTIL_UTIL_HPP