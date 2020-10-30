#include <iostream>

#include "../Eve.hpp"

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr
        << "Invalid args. Possible Usage:\nEve-Client GET <address> <port> "
           "<e-mail_id>\nEve-Client GET <address> <port> <e-mail_id> "
           "status\nEve-Client POST <address> <port> <from_email> <to_email> "
           "<subject> <body>\nEve-Client DELETE <address> <port> <e-mail_id>\n";
    return 1;
  }

  const std::string method = argv[1];
  eve::client::Eve client(argv[2], argv[3]);
  try
  {
    if (method == "GET")
    {
      if (argc < 5)
      {
        std::cerr << "Invalid GET args. Usage:\nEve-Client GET <address> "
                     "<port> <e-mail_id>\n";
        return 1;
      }
      const auto email = client.get_email(argv[4]);
      std::cout << "E-mail data:\n" << email << std::endl;
      return 0;
    }
    else if (method == "GET_STATUS")
    {
      if (argc < 5)
      {
        std::cerr << "Invalid GET_STATUS args. Usage:\nEve-Client GET_STATUS "
                     "<address> <port> <e-mail_id>\n";
        return 1;
      }
      const auto status = client.get_email_status(argv[4]);
      std::cout << "E-mail status: " << status << std::endl;
      return 0;
    }
    else if (method == "POST")
    {
      if (argc < 8)
      {
        std::cerr << "Invalid POST args. Usage:\nEve-Client GET "
                     "<address> <port> "
                     "<e-mail_id>\nEve-Client POST <address> <port> "
                     "<from_email> <to_email> "
                     "<subject> <body>\n";
        return 1;
      }

      const auto uuid = client.send_email(argv[4], argv[5], argv[6], argv[7]);
      std::cout << "E-mail stored on server with id " << uuid << std::endl;

      std::string status = client.get_email_status(uuid);
      std::cout << "E-mail status: " << status << std::endl;
      while (status != "Sent" && status != "Gave up")
      {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        status = client.get_email_status(uuid);
        std::cout << "E-mail status: " << status << std::endl;
      }

      return 0;
    }
    else if (method == "DELETE")
    {
      if (argc < 5)
      {
        std::cerr
            << "Invalid DELETE args. Usage:\nEve-Client GET "
               "<address> <port> "
               "<e-mail_id>\nEve-Client DELETE <address> <port> <e-mail_id>\n";
        return 1;
      }

      if (client.delete_email(argv[4]))
      {
        std::cout << "E-mail successfully deleted." << std::endl;
      }
      else
      {
        std::cout << "E-mail not found." << std::endl;
      }
      return 0;
    }
    else
    {
      std::cerr
          << "Invalid args. Possible Usage:\nEve-Client GET <address> <port> "
             "<e-mail_id>\nEve-Client GET <address> <port> <e-mail_id> "
             "status\nEve-Client POST <address> <port> <from_email> <to_email> "
             "<subject> <body>\nEve-Client DELETE <address> <port> "
             "<e-mail_id>\n";
      return 1;
    }
  }
  catch (const std::exception& exception)
  {
    std::cerr << "Error: " << exception.what() << std::endl;
    return -1;
  }
}