#ifndef USERS_CONNECTIONS_SERVER_HPP_
#define USERS_CONNECTIONS_SERVER_HPP_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
// #include "src/nlohmann/json.hpp"
// #include "concurrentqueue.h"

using namespace boost::asio;

namespace p2p {

struct Connection {
  boost::asio::ip::tcp::socket socket;
  boost::asio::streambuf read_buffer;
  Connection(boost::asio::io_service &io_service)
      : socket(io_service), read_buffer() {}
  Connection(boost::asio::io_service &io_service, size_t max_buffer_size)
      : socket(io_service), read_buffer(max_buffer_size) {}
};

class UsersConnectionsServer {
  boost::asio::io_service m_ioservice;
  boost::asio::ip::tcp::acceptor m_acceptor;
  std::list<Connection> m_connections;
  using con_handle_t = std::list<Connection>::iterator;

public:
  UsersConnectionsServer()
      : m_ioservice(), m_acceptor(m_ioservice), m_connections() {}

  void handle_read(con_handle_t con_handle,
                   boost::system::error_code const &err,
                   size_t bytes_transfered) {
    if (bytes_transfered > 0) {
      std::istream is(&con_handle->read_buffer);
      std::string line;
      std::getline(is, line);
      std::cout << "Message Received: " << line << std::endl; // Для дебага
      if (line == "need update") {
        // Если получено "need update, проверяем наличие и отправляем либо "No
        // updates\n", либо сам апдейт

        std::shared_ptr<std::string> response =
            std::make_shared<std::string>("No updates");
        auto handler =
            boost::bind(&UsersConnectionsServer::handle_write, this, con_handle,
                        response, boost::asio::placeholders::error);
        boost::asio::async_write(con_handle->socket,
                                 boost::asio::buffer(*response), handler);
      } else {
        // Остальные случаи пока не обрабатываем(есть ли они вообще?)

        std::shared_ptr<std::string> response =
            std::make_shared<std::string>("ping");
        auto handler =
            boost::bind(&UsersConnectionsServer::handle_write, this, con_handle,
                        response, boost::asio::placeholders::error);
        boost::asio::async_write(con_handle->socket,
                                 boost::asio::buffer(*response), handler);
      }
    }
    if (!err) {
      do_async_read(con_handle);
    } else {
      std::cerr
          << "We had an error: " << err.message()
          << std::endl; // Возможно, здесь будет более умная обработка ошибок
      m_connections.erase(con_handle);
    }
  }

  void do_async_read(con_handle_t con_handle) {
    auto handler = boost::bind(&UsersConnectionsServer::handle_read, this,
                               con_handle, boost::asio::placeholders::error,
                               boost::asio::placeholders::bytes_transferred);
    boost::asio::async_read_until(con_handle->socket, con_handle->read_buffer,
                                  "\n", handler);
  }

  void handle_write(con_handle_t con_handle,
                    std::shared_ptr<std::string> msg_buffer,
                    boost::system::error_code const &err) {
    if (!err) {
      std::cout << "Finished sending message\n"; // Для дебага
      if (con_handle->socket.is_open()) {
        // Всё супер - ответ написан, подключение открыто
      }
    } else {
      std::cerr
          << "We had an error: " << err.message()
          << std::endl; // Возможно, здесь будет более умная обработка ошибок
      m_connections.erase(con_handle);
    }
  }

  void handle_accept(con_handle_t con_handle,
                     boost::system::error_code const &err) {
    if (!err) {
      std::cout << "Connection from: "
                << con_handle->socket.remote_endpoint().address().to_string()
                << "\n";                // Для дебага
      std::cout << "Sending message\n"; // Для дебага
      auto buff = std::make_shared<std::string>("Hello World!\n");
      auto handler =
          boost::bind(&UsersConnectionsServer::handle_write, this, con_handle,
                      buff, boost::asio::placeholders::error);
      boost::asio::async_write(con_handle->socket, boost::asio::buffer(*buff),
                               handler);
      do_async_read(con_handle);
    } else {
      std::cerr
          << "We had an error: " << err.message()
          << std::endl; // Возможно, здесь будет более умная обработка ошибок
      m_connections.erase(con_handle);
    }
    start_accept();
  }

  void start_accept() {
    auto con_handle = m_connections.emplace(m_connections.begin(), m_ioservice);
    auto handler = boost::bind(&UsersConnectionsServer::handle_accept, this,
                               con_handle, boost::asio::placeholders::error);
    m_acceptor.async_accept(con_handle->socket, handler);
  }

  void listen(uint16_t port) {
    auto endpoint =
        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
    m_acceptor.open(endpoint.protocol());
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.bind(endpoint);
    m_acceptor.listen();
    start_accept();
  }

  void run() { m_ioservice.run(); }
};

} // namespace p2p

#endif // USERS_CONNECTIONS_SERVER_HPP_
