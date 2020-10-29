#include "Email_storage.hpp"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>

#include "Email_not_found_exception.hpp"

namespace eve
{
namespace server
{
namespace data
{
Email_storage& Email_storage::get_instance()
{
  static Email_storage instance;
  return instance;
}

boost::uuids::uuid Email_storage::add_email(const nlohmann::json& email)
{
  std::lock_guard<std::mutex> guard(m_map_mutex);

  const auto uuid = boost::uuids::random_generator()();
  m_emails.insert_or_assign(uuid, email);

  return uuid;
}

nlohmann::json Email_storage::find_email(const boost::uuids::uuid& id) const
{
  std::lock_guard<std::mutex> guard(m_map_mutex);
  return find_email_L(id);
}

bool Email_storage::remove_email(const boost::uuids::uuid& id)
{
  std::lock_guard<std::mutex> guard(m_map_mutex);
  return m_emails.erase(id) == 1;
}

void Email_storage::change_email_status(
    const boost::uuids::uuid& id,
    const Data_definition::Email_status& new_status)
{
  std::lock_guard<std::mutex> guard(m_map_mutex);
  const auto it = m_emails.find(id);
  if (it == m_emails.end())
  {
    const auto string_uuid = boost::uuids::to_string(id);
    throw Email_not_found_exception("E-mail with id " + string_uuid +
                                    " not found");
  }
  it->second["status"] = Data_definition::to_string(new_status);
}

Email_storage::storage_const_iterator Email_storage::cbegin() const
{
  return m_emails.cbegin();
}
Email_storage::storage_const_iterator Email_storage::cend() const
{
  return m_emails.cend();
}

std::string Email_storage::get_email_status(const boost::uuids::uuid& id) const
{
  std::lock_guard<std::mutex> guard(m_map_mutex);
  const auto json_email = find_email_L(id);
  return json_email["status"];
}

Email_storage::Email_storage() {}

nlohmann::json Email_storage::find_email_L(const boost::uuids::uuid& id) const
{
  const auto it = m_emails.find(id);
  if (it == m_emails.end())
  {
    throw Email_not_found_exception("E-mail not found");
  }
  return it->second;
}

}  // namespace data
}  // namespace server
}  // namespace eve