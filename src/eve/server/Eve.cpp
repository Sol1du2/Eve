#include "Eve.hpp"

#include <iostream>

#include "config/Eve_config.hpp"

namespace eve
{
namespace server
{
Eve::Eve(const std::string& address, const std::string& port,
         const std::string& smtp_address, const std::string& smtp_port)
    : m_io_context(1), m_tcp_acceptor(m_io_context), m_signals(m_io_context)
{
  config::eve_config.eve_address = address;
  config::eve_config.eve_port = port;
  config::eve_config.smtp_address = smtp_address;
  config::eve_config.smtp_port = smtp_port;

  m_signals.add(SIGINT);
  m_signals.add(SIGTERM);
#ifdef SIGQUIT
  m_signals.add(SIGQUIT);
#endif

  wait_signals();

  boost::asio::ip::tcp::resolver resolver(m_io_context);
  boost::asio::ip::tcp::endpoint endpoint =
      *resolver.resolve(address, port).begin();
  m_tcp_acceptor.open(endpoint.protocol());
  m_tcp_acceptor.set_option(
      boost::asio::ip::tcp::acceptor::reuse_address(true));
  m_tcp_acceptor.bind(endpoint);
  m_tcp_acceptor.listen();

  accept_connections();
}

void Eve::run() { m_io_context.run(); }

void Eve::accept_connections()
{
  m_tcp_acceptor.async_accept([&](boost::system::error_code ec,
                                  boost::asio::ip::tcp::socket socket) {
    if (!m_tcp_acceptor.is_open())
    {
      return;
    }

    if (!ec)
    {
      std::cout << "Connection established" << std::endl;

      auto connection = std::make_shared<Connection>(std::move(socket));
      connection->on_ready_close([&](const std::shared_ptr<Connection>& connection) {
        std::cout << "Erasing Connection..." << std::endl;
        m_connections.erase(connection);
        connection->close();
      });
      m_connections.emplace(connection);

      connection->open();
    }

    accept_connections();
  });

  std::cout << "Ready to accept connections!" << std::endl;
}

void Eve::wait_signals()
{
  m_signals.async_wait([this](boost::system::error_code /*ec*/, int /*signo*/) {
    std::cout << "Closing all connections..." << std::endl;
    m_tcp_acceptor.close();
    close_all_connections();
  });
}

void Eve::close_all_connections()
{
  for (const auto& connection : m_connections)
  {
    connection->close();
  }
  m_connections.clear();
}

}  // namespace server
}  // namespace eve