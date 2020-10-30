#include <iostream>

#include "../Eve.hpp"

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr
        << "Usage: Eve-Server <address> <port> <smtp_address> <smtp_port>\n";
    return 1;
  }

  try
  {
    eve::server::Eve server(argv[1], argv[2], argv[3], argv[4]);
    server.run();
    std::cout << "Eve gracefully shutting down..." << std::endl;
  }
  catch (const std::exception& exception)
  {
    std::cerr << "Error: " << exception.what() << std::endl;
    return -1;
  }

  return 0;
}