#include "Json_handler.hpp"

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

#include "../../util/Util.hpp"
#include "../config/Eve_config.hpp"
#include "../data/Email_not_found_exception.hpp"
#include "../data/Email_storage.hpp"
#include "../smtp/Smtp_client.hpp"

namespace eve
{
namespace server
{
namespace api
{
namespace
{
constexpr const char* const HANDLER_URI = "/jsonApi";
}

std::string Json_handler::get_handler_uri() { return HANDLER_URI; }

Json_handler::Json_handler() {}

eve::msg::Reply Json_handler::handle(const eve::msg::Request& request,
                                     const std::string& args) const
{
  nlohmann::json json_reply;
  auto& data_storage = eve::server::data::Email_storage::get_instance();
  if (request.method == "GET")
  {
    return handle_get(args);
  }
  else if (request.method == "POST")
  {
    return handle_post(request.content);
  }
  else if (request.method == "DELETE")
  {
    return handle_delete(args);
  }

  return eve::msg::Reply::stock_reply(
      eve::msg::Reply::Status_type::Not_implemented);
}

eve::msg::Reply Json_handler::handle_get(const std::string& args) const
{
  nlohmann::json json_reply;
  auto& data_storage = eve::server::data::Email_storage::get_instance();

  if (!args.empty())
  {
    std::vector<std::string> split_args =
        eve::util::Util::split_string("&", args);
    std::vector<std::string> split_id_arg =
        eve::util::Util::split_string("=", split_args.front());
    if (split_id_arg.size() == 2u)
    {
      try
      {
        const auto uuid = boost::uuids::string_generator()(split_id_arg.back());
        if (split_args.size() == 1u)
        {
          json_reply = data_storage.find_email(uuid);
        }
        else if (split_args.size() > 1u && split_args[1] == "status_only")
        {
          json_reply["status"] = data_storage.get_email_status(uuid);
        }
        else
        {
          return eve::msg::Reply::stock_reply(
              eve::msg::Reply::Status_type::Bad_request);
        }
      }
      catch (const std::exception&)
      {
        json_reply["not_found"] = "Email message not found";
      }
    }
    else
    {
      return eve::msg::Reply::stock_reply(
          eve::msg::Reply::Status_type::Bad_request);
    }
  }
  else
  {
    auto storage_it = data_storage.cbegin();
    while (storage_it != data_storage.cend())
    {
      const auto uuid = boost::uuids::to_string(storage_it->first);
      json_reply[uuid] = storage_it->second;
      ++storage_it;
    }
  }

  std::stringstream stream;
  stream << json_reply;

  return ok_reply(stream.str());
}
eve::msg::Reply Json_handler::handle_post(const std::string& request_body) const
{
  nlohmann::json json_body;
  try
  {
    json_body = nlohmann::json::parse(request_body);
  }
  catch (const nlohmann::json::parse_error&)
  {
    return eve::msg::Reply::stock_reply(
        eve::msg::Reply::Status_type::Bad_request);
  }

  const auto from_email_it = json_body.find("from_email");
  const auto to_email_it = json_body.find("to_email");
  const auto subject_it = json_body.find("subject");
  const auto body_it = json_body.find("body");
  if (from_email_it == json_body.end() || to_email_it == json_body.end() ||
      subject_it == json_body.end() || body_it == json_body.end() ||
      json_body.size() != 4u)
  {
    return eve::msg::Reply::stock_reply(
        eve::msg::Reply::Status_type::Bad_request);
  }

  auto& data_storage = eve::server::data::Email_storage::get_instance();
  json_body["status"] = "Waiting";
  const auto uuid = data_storage.add_email(json_body);

  send_smtp_email(*from_email_it, *to_email_it, *subject_it, *body_it, uuid);

  nlohmann::json json_reply;
  json_reply["id"] = boost::uuids::to_string(uuid);
  std::stringstream stream;
  stream << json_reply;

  return ok_reply(stream.str());
}
eve::msg::Reply Json_handler::handle_delete(const std::string& args) const
{
  auto& data_storage = eve::server::data::Email_storage::get_instance();
  nlohmann::json json_reply;

  std::vector<std::string> split_args =
      eve::util::Util::split_string("=", args);
  if (split_args.size() > 1 && split_args.front() == "id")
  {
    try
    {
      const auto uuid = boost::uuids::string_generator()(split_args.back());
      if (data_storage.remove_email(uuid))
      {
        json_reply["deleted"] = "Email message deleted";
      }
      else
      {
        json_reply["not_found"] = "Email message not found";
      }
    }
    catch (const std::exception&)
    {
      json_reply["not_found"] = "Email message not found";
    }
  }
  else
  {
    return eve::msg::Reply::stock_reply(
        eve::msg::Reply::Status_type::Bad_request);
  }

  std::stringstream stream;
  stream << json_reply;

  return ok_reply(stream.str());
}

eve::msg::Reply Json_handler::ok_reply(const std::string& content) const
{
  eve::msg::Reply reply;
  reply.status = eve::msg::Reply::Status_type::Ok;

  reply.content = content;
  reply.headers.resize(2);
  reply.headers[0].name = "Content-Length";
  reply.headers[0].value = std::to_string(reply.content.size());
  reply.headers[1].name = "Content-Type";
  reply.headers[1].value = "application/json";

  return reply;
}

void Json_handler::send_smtp_email(const std::string& from_email,
                                   const std::string& to_email,
                                   const std::string& subject,
                                   const std::string& body,
                                   const boost::uuids::uuid& email_uuid) const
{
  std::thread smtp_thread(
      [](const std::string from_email, const std::string to_email,
         const std::string subject, const std::string body,
         const boost::uuids::uuid email_uuid) {
        auto& storage = eve::server::data::Email_storage::get_instance();

        int try_number = 0;
        bool succeeded = false;
        while (try_number < config::eve_config.num_tries_smtp_email &&
               !succeeded)
        {
          try
          {
            std::cout << "Attempting to send e-mail to SMTP server..."
                      << std::endl;
            storage.change_email_status(
                email_uuid,
                eve::server::data::Data_definition::Email_status::Sending);

            eve::server::smtp::Smtp_client smtp_client(
                config::eve_config.eve_address, config::eve_config.smtp_address,
                config::eve_config.smtp_port);
            smtp_client.send_message(from_email, to_email, subject, body);

            std::cout << "E-mail sent!" << std::endl;
            storage.change_email_status(
                email_uuid,
                eve::server::data::Data_definition::Email_status::Sent);
            succeeded = true;
          }
          catch (const eve::server::data::Email_not_found_exception& exception)
          {
            // E-mail could've been removed by the user, no point in trying
            // again.
            std::cerr << exception.what() << std::endl;
            return;
          }
          catch (const std::exception& exception)
          {
            std::cerr << "Error: " << exception.what()
                      << "\nAttempt number: " << try_number << "/"
                      << config::eve_config.num_tries_smtp_email << std::endl;
            storage.change_email_status(
                email_uuid,
                eve::server::data::Data_definition::Email_status::Failed);

            // Wait 2 minutes and then try again. Unless this was the last try.
            if (try_number + 1 < config::eve_config.num_tries_smtp_email)
            {
              std::this_thread::sleep_for(std::chrono::minutes(2));
            }
          }

          ++try_number;
        }

        if (!succeeded)
        {
          std::cerr << "That was the last attempt! I'm giving up! :("
                    << std::endl;
          storage.change_email_status(
              email_uuid,
              eve::server::data::Data_definition::Email_status::Gave_up);
        }
      },
      from_email, to_email, subject, body, email_uuid);

  smtp_thread.detach();
}

}  // namespace api
}  // namespace server
}  // namespace eve