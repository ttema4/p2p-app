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

#include "storage_structures.hpp"

using namespace boost::asio;

std::unordered_map<std::string, std::string> p2p::last_version_by_user;

namespace p2p {

struct Connection {
  boost::asio::ip::tcp::socket socket;
  boost::asio::streambuf read_buffer;
  Connection(boost::asio::io_context &io_context)
      : socket(io_context), read_buffer() {}
  Connection(boost::asio::io_context &io_context, size_t max_buffer_size)
      : socket(io_context), read_buffer(max_buffer_size) {}
};

class UsersConnectionsServer {
  boost::asio::io_context &m_ioservice;
  boost::asio::ip::tcp::acceptor m_acceptor;
  std::list<Connection> m_connections;
  using con_handle_t = std::list<Connection>::iterator;

public:
  UsersConnectionsServer(boost::asio::io_context &io_context)
      : m_ioservice(io_context), m_acceptor(m_ioservice), m_connections() {}

  void handle_read(con_handle_t con_handle,
                   boost::system::error_code const &err,
                   size_t bytes_transfered) {
    if (bytes_transfered > 0) {
      std::istream is(&con_handle->read_buffer);
      std::string line;
      std::getline(is, line);
      if (USERS_SERVER_LOGS_ON) {
        std::cout << "SERVER: Message Received: " << line << std::endl;
      }
      if (line == "need update") {
        std::string address =
            con_handle->socket.remote_endpoint().address().to_string();
        std::shared_ptr<std::string> response;
        std::string current_up_to_date = up_to_date_version.get();
        if (last_version_by_user.find(address) == last_version_by_user.end() ||
            last_version_by_user[address] != current_up_to_date) {
          response = std::make_shared<std::string>(current_up_to_date + "\n");
          last_version_by_user[address] = current_up_to_date;
        } else {
          response = std::make_shared<std::string>("no updates\n");
        }
        auto handler =
            boost::bind(&UsersConnectionsServer::handle_write, this, con_handle,
                        response, boost::asio::placeholders::error);
        boost::asio::async_write(con_handle->socket,
                                 boost::asio::buffer(*response), handler);
      } else {
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
      std::cerr << "SERVER: We had an error: " << err.message() << std::endl;
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
      if (USERS_SERVER_LOGS_ON) {
        std::cout << "SERVER: Finished sending message to client\n";
      }
      if (con_handle->socket.is_open()) {
        // Всё супер - ответ написан, подключение открыто
      }
    } else {
      std::cerr << "SERVER: We had an error: " << err.message() << std::endl;
      m_connections.erase(con_handle);
    }
  }

  void handle_accept(con_handle_t con_handle,
                     boost::system::error_code const &err) {
    if (!err) {
      if (USERS_SERVER_LOGS_ON) {
        std::cout << "SERVER: Connection from: "
                  << con_handle->socket.remote_endpoint().address().to_string()
                  << "\n";
        std::cout << "SERVER: Sending message\n";
      }
      std::string current_up_to_date = up_to_date_version.get();
      last_version_by_user[con_handle->socket.remote_endpoint().address().to_string()] = current_up_to_date;
      auto buff = std::make_shared<std::string>(current_up_to_date + "\n");
      auto handler =
          boost::bind(&UsersConnectionsServer::handle_write, this, con_handle,
                      buff, boost::asio::placeholders::error);
      boost::asio::async_write(con_handle->socket, boost::asio::buffer(*buff),
                               handler);
      do_async_read(con_handle);
    } else {
      std::cerr << "SERVER: We had an error: " << err.message() << std::endl;
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
