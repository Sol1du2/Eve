#ifndef EVE_SERVER_DATA_EMAIL_STORAGE_HPP
#define EVE_SERVER_DATA_EMAIL_STORAGE_HPP

#include <boost/uuid/uuid.hpp>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>

#include "Data_definition.hpp"

namespace eve
{
namespace server
{
namespace data
{
/// This class represents the server's storage. At the moment the storage is
/// only present in memory and nothing is saved to disk. The methods are
/// thread-safe, except the private ones marked with _L.
class Email_storage
{
 public:
  using storage_const_iterator =
      std::map<boost::uuids::uuid, nlohmann::json>::const_iterator;

  static Email_storage& get_instance();

  Email_storage(Email_storage const&) = delete;
  void operator=(Email_storage const&) = delete;

  boost::uuids::uuid add_email(const nlohmann::json& email);
  nlohmann::json find_email(const boost::uuids::uuid& id) const;
  bool remove_email(const boost::uuids::uuid& id);

  void change_email_status(const boost::uuids::uuid& id,
                           const Data_definition::Email_status& status);

  storage_const_iterator cbegin() const;
  storage_const_iterator cend() const;

  std::string get_email_status(const boost::uuids::uuid& id) const;

 private:
  Email_storage();

  nlohmann::json find_email_L(const boost::uuids::uuid& id) const;

  mutable std::mutex m_map_mutex;
  std::map<boost::uuids::uuid, nlohmann::json> m_emails;
};

}  // namespace data
}  // namespace server
}  // namespace eve

#endif  // EVE_SERVER_DATA_EMAIL_STORAGE_HPP