#ifndef EVE_SERVER_CONFIG_EVE_CONFIG_HPP
#define EVE_SERVER_CONFIG_EVE_CONFIG_HPP

#include <string>

namespace eve
{
namespace server
{
namespace config
{
struct Eve_config
{
  std::string eve_address;
  std::string eve_port;

  std::string smtp_address;
  std::string smtp_port;

  int num_tries_smtp_email = 5;
};

inline Eve_config eve_config;

}  // namespace config
}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_CONFIG_EVE_CONFIG_HPP