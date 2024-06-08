#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include "parser_connection_client.hpp"
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <list>
#include <thread>
#include "storage_structures.hpp"

namespace p2p {

ParserConnectionClient::ParserConnectionClient(
    boost::asio::io_context &io_context,
    const ip::tcp::resolver::results_type &endpoints
)
    : io_context_(io_context), socket_(io_context), endpoints_(endpoints) {
    do_connect(endpoints);
}

void ParserConnectionClient::start() {
    start_receive();
    start_check_updates();
}

void ParserConnectionClient::start_check_updates() {
    check_timer.expires_after(std::chrono::seconds(1));
    check_timer.async_wait([this](boost::system::error_code ec) {
        if (!ec) {
            send_request("need update");
            start_check_updates();
        }
    });
}

void ParserConnectionClient::send_request(const std::string &request) {
    std::ofstream log_file(std::string(LOGS_PATH) + "/logs.txt");
    if (PARSER_LOGS_ON) {
        log_file << "PARSER'S CLIENT: Sending request: " << request << std::endl;
    }
    async_write(socket_, buffer(request + '\n'), [this](boost::system::error_code ec, std::size_t /*length*/) {
        if (ec) {
            std::ofstream log_file(std::string(LOGS_PATH) + "/logs.txt");
            log_file << "PARSER'S CLIENT: Error sending request: " << ec.message() << std::endl;
        }
    });
}

void ParserConnectionClient::start_receive() {
    async_read_until(socket_, response_, '\n', [this](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            std::istream response_stream(&response_);
            std::string response;
            std::getline(response_stream, response);
            if (response != "Hello World!") {
                parsers_responses.enqueue(response);
            }
            start_receive();
        } else {
            std::ofstream log_file(std::string(LOGS_PATH) + "/logs.txt");
            log_file << "PARSER'S CLIENT: Error receiving response: " << ec.message() << std::endl;
        }
    });
}

void ParserConnectionClient::do_connect(const ip::tcp::resolver::results_type &endpoints) {
    async_connect(socket_, endpoints, [this](boost::system::error_code ec, ip::tcp::endpoint) {
        if (!ec) {
            if (PARSER_LOGS_ON) {
                std::ofstream log_file(std::string(LOGS_PATH) + "/logs.txt");
                log_file << "PARSER'S CLIENT: Connected to parser's server." << std::endl;
            }
            start();
        } else {
            if (ec.message() == "Connection refused") {
                std::ofstream log_file(std::string(LOGS_PATH) + "/logs.txt");
                log_file << "PARSER'S CLIENT: Error connecting to server: "
                            "Connection refused"
                         << std::endl;
                log_file << "PARSER'S CLIENT: Trying to reconnect..." << std::endl;
                do_reconnect();
            } else {
                std::ofstream log_file(std::string(LOGS_PATH) + "/logs.txt");
                log_file << "PARSER'S CLIENT: Error connecting to server: " << ec.message() << std::endl;
            }
        }
    });
}

void ParserConnectionClient::do_reconnect() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    socket_.close();
    io_context_.post([this]() { do_connect(endpoints_); });
}

}  // namespace p2p
