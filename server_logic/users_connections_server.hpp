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

namespace p2p {

struct Connection {
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf read_buffer;
    Connection(boost::asio::io_context &io_context);
    Connection(boost::asio::io_context &io_context, size_t max_buffer_size);
};

class UsersConnectionsServer {
    boost::asio::io_context &m_ioservice;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::list<Connection> m_connections;
    using con_handle_t = std::list<Connection>::iterator;

public:
    UsersConnectionsServer(boost::asio::io_context &io_context);

    void handle_read(con_handle_t con_handle, const boost::system::error_code &err, size_t bytes_transfered);
    void do_async_read(con_handle_t con_handle);
    void handle_write(
        con_handle_t con_handle,
        std::shared_ptr<std::string> msg_buffer,
        const boost::system::error_code &err
    );
    void handle_accept(con_handle_t con_handle, const boost::system::error_code &err);
    void start_accept();
    void listen(uint16_t port);
    void run();
};

}  // namespace p2p

#endif  // USERS_CONNECTIONS_SERVER_HPP_
