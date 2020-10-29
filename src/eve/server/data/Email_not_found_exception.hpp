#ifndef EVE_SERVER_DATA_EMAIL_NOT_FOUND_EXCEPTION_HPP
#define EVE_SERVER_DATA_EMAIL_NOT_FOUND_EXCEPTION_HPP

#include "../../util/Eve_exception.hpp"

namespace eve
{
namespace server
{
namespace data
{
class Email_not_found_exception : public eve::util::Eve_exception
{
 public:
  using eve::util::Eve_exception::Eve_exception;
};

}  // namespace data
}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_DATA_EMAIL_NOT_FOUND_EXCEPTION_HPP