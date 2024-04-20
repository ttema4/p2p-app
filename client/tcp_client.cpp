#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

class Client {
public:
    Client(boost::asio::io_context& io_context, const ip::tcp::resolver::results_type& endpoints)
        : io_context_(io_context), socket_(io_context) {
        do_connect(endpoints);
    }

    void start() {
        start_receive();
        start_check_updates();
    }

private:
    void start_check_updates() {
        check_timer.expires_after(std::chrono::seconds(5)); // Проверяем обновления каждые 5 секунд
        check_timer.async_wait([this](boost::system::error_code ec) {
            if (!ec) {
                send_request("need update");
                start_check_updates(); // Повторно запускаем проверку обновлений
            }
        });
    }

    void send_request(const std::string& request) {
        std::cout << "Sending request: " << request << std::endl;
        async_write(socket_, buffer(request + "\n"), [this](boost::system::error_code ec, std::size_t /*length*/) {
        if (ec) {
            std::cerr << "Error sending request: " << ec.message() << std::endl;
        }
        });
    }

    void start_receive() {
        async_read_until(socket_, response_, '\n', [this](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::istream response_stream(&response_);
                std::string response;
                std::getline(response_stream, response);
                std::cout << "Received response: " << response << std::endl;
                // Здесь можно обработать полученные данные, если это необходимо
                start_receive();
            } else {
                std::cerr << "Error receiving response: " << ec.message() << std::endl;
            }
        });
    }

    void do_connect(const ip::tcp::resolver::results_type& endpoints) {
        async_connect(socket_, endpoints, [this](boost::system::error_code ec, ip::tcp::endpoint) {
            if (!ec) {
                std::cout << "Connected to server." << std::endl;
                start();
            } else {
                std::cerr << "Error connecting to server: " << ec.message() << std::endl;
            }
        });
    }

private:
    boost::asio::io_context& io_context_;
    ip::tcp::socket socket_;
    boost::asio::streambuf response_;
    boost::asio::steady_timer check_timer{io_context_};
};

int main() {
    try {
        boost::asio::io_context io_context;
        ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("localhost", "12345");

        Client client(io_context, endpoints);

        // Run the io_context to start asynchronous operations
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
