#ifndef EVE_SERVER_UTIL_EVE_EXCEPTION_HPP
#define EVE_SERVER_UTIL_EVE_EXCEPTION_HPP

#include <exception>
#include <string>

namespace eve
{
namespace util
{
/// All exceptions in the project should extend the Eve_exception
class Eve_exception : public std::exception
{
 public:
  Eve_exception(const std::string& error);

  const char* what() const noexcept override;

 private:
  std::string m_error;
};

}  // namespace util
}  // namespace eve

#endif  // EVE_SERVER_UTIL_EVE_EXCEPTION_HPP