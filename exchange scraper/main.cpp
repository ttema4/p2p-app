#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <list>
#include <mutex>
#include <thread>
#include "scraper.hpp"

std::mutex m;

struct Connection {
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf read_buffer;

    Connection(boost::asio::io_service &io_service)
        : socket(io_service), read_buffer() {
    }

    Connection(boost::asio::io_service &io_service, size_t max_buffer_size)
        : socket(io_service), read_buffer(max_buffer_size) {
    }
};

class Server {
    boost::asio::io_service m_ioservice;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::list<Connection> m_connections;
    using con_handle_t = std::list<Connection>::iterator;

public:
    Server() : m_ioservice(), m_acceptor(m_ioservice), m_connections() {
    }

    void handle_read(
        con_handle_t con_handle,
        const boost::system::error_code &err,
        size_t bytes_transfered
    ) {
        if (bytes_transfered > 0) {
            std::istream is(&con_handle->read_buffer);
            std::string line;
            getline(is, line);
            std::cout << "Message Received: " << line << ", " << line.size()
                      << std::endl;
            if (line == "need update") {
                std::unique_lock l(m);
                // Если получено "need update", отправляем сообщение о том, что
                // апдейта нет
                std::string back;
                std::ifstream file_of_orders;
                file_of_orders.open("orders.json");
                nlohmann::json json_orders{};
                file_of_orders >> json_orders;
                file_of_orders.close();
                back = json_orders.dump() + '\n';
                std::cout << "sending: " << back << "\n";
                std::shared_ptr<std::string> response =
                    std::make_shared<std::string>(back);
                auto handler = boost::bind(
                    &Server::handle_write, this, con_handle, response,
                    boost::asio::placeholders::error
                );
                boost::asio::async_write(
                    con_handle->socket, boost::asio::buffer(*response), handler
                );
            } else {
                // Во всех остальных случаях отправляем "ping"
                std::shared_ptr<std::string> response =
                    std::make_shared<std::string>("ping\r\n");
                auto handler = boost::bind(
                    &Server::handle_write, this, con_handle, response,
                    boost::asio::placeholders::error
                );
                boost::asio::async_write(
                    con_handle->socket, boost::asio::buffer(*response), handler
                );
            }
        }

        if (!err) {
            do_async_read(con_handle);
        } else {
            std::cerr << "We had an error: " << err.message() << std::endl;
            m_connections.erase(con_handle);
        }
    }

    void do_async_read(con_handle_t con_handle) {
        auto handler = boost::bind(
            &Server::handle_read, this, con_handle,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        );
        boost::asio::async_read_until(
            con_handle->socket, con_handle->read_buffer, '\n', handler
        );
    }

    void handle_write(
        con_handle_t con_handle,
        std::shared_ptr<std::string> msg_buffer,
        const boost::system::error_code &err
    ) {
        if (!err) {
            std::cout << "Finished sending message\n";
            if (con_handle->socket.is_open()) {
                // Write completed successfully and connection is open
            }
        } else {
            std::cerr << "We had an error: " << err.message() << std::endl;
            m_connections.erase(con_handle);
        }
    }

    void handle_accept(
        con_handle_t con_handle,
        const boost::system::error_code &err
    ) {
        if (!err) {
            std::cout
                << "Connection from: "
                << con_handle->socket.remote_endpoint().address().to_string()
                << "\n";
            std::cout << "Sending message\n";
            auto buff = std::make_shared<std::string>("Hello World!\r\n\r\n");
            auto handler = boost::bind(
                &Server::handle_write, this, con_handle, buff,
                boost::asio::placeholders::error
            );
            boost::asio::async_write(
                con_handle->socket, boost::asio::buffer(*buff), handler
            );
            do_async_read(con_handle);
        } else {
            std::cerr << "We had an error: " << err.message() << std::endl;
            m_connections.erase(con_handle);
        }
        start_accept();
    }

    void start_accept() {
        auto con_handle =
            m_connections.emplace(m_connections.begin(), m_ioservice);
        auto handler = boost::bind(
            &Server::handle_accept, this, con_handle,
            boost::asio::placeholders::error
        );
        m_acceptor.async_accept(con_handle->socket, handler);
    }

    void listen(uint16_t port) {
        auto endpoint =
            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
        m_acceptor.open(endpoint.protocol());
        m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true
        ));
        m_acceptor.bind(endpoint);
        m_acceptor.listen();
        start_accept();
    }

    void run() {
        m_ioservice.run();
    }
};

int main(int, char **) {
    auto fake_exchange = std::make_unique<bybit_simulator>();
    scraper scraper_;
    scraper_.add_exchange(std::move(fake_exchange));
    std::thread t([&]() {
        while (true) {
            std::unique_lock l(m);
            scraper_.update_exchanges_orders();
            scraper_.handle();
            l.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });
    t.detach();
    auto srv = Server();
    srv.listen(12345);
    srv.run();
}