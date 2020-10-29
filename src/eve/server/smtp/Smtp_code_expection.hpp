#ifndef EVE_SERVER_SMTP_SMTP_CODE_EXCEPTION_HPP
#define EVE_SERVER_SMTP_SMTP_CODE_EXCEPTION_HPP

#include "../../util/Eve_exception.hpp"

namespace eve
{
namespace server
{
namespace smtp
{
class Smtp_code_exception : public eve::util::Eve_exception
{
 public:
  using eve::util::Eve_exception::Eve_exception;
};

}  // namespace smtp
}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_SMTP_SMTP_CODE_EXCEPTION_HPP