#include "Eve_exception.hpp"

namespace eve
{
namespace util
{
Eve_exception::Eve_exception(const std::string& error) : m_error(error) {}

const char* Eve_exception::what() const noexcept { return m_error.c_str(); }

}  // namespace util
}  // namespace eve
