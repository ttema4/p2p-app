#ifndef PARSER_CONNECTION_CLIENT_HPP_
#define PARSER_CONNECTION_CLIENT_HPP_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace boost::asio;

namespace p2p {

class ParserConnectionClient {
public:
  ParserConnectionClient(boost::asio::io_context &io_context,
                         const ip::tcp::resolver::results_type &endpoints)
      : io_context_(io_context), socket_(io_context) {
    do_connect(endpoints);
  }

  void start() {
    start_receive();
    start_check_updates();
  }

private:
  void start_check_updates() {
    check_timer.expires_after(std::chrono::seconds(1));
    check_timer.async_wait([this](boost::system::error_code ec) {
      if (!ec) {
        send_request("need update");
        start_check_updates();
      }
    });
  }

  void send_request(const std::string &request) {
    if (PARSER_LOGS_ON) {
      std::cout << "PARSER'S CLIENT: Sending request: " << request << std::endl;
    }
    async_write(socket_, buffer(request + '\n'),
                [this](boost::system::error_code ec, std::size_t /*length*/) {
                  if (ec) {
                    std::cerr << "PARSER'S CLIENT: Error sending request: "
                              << ec.message() << std::endl;
                  }
                });
  }

  void start_receive() {
    async_read_until(socket_, response_, '\n',
                     [this](boost::system::error_code ec, std::size_t length) {
                       if (!ec) {
                         std::istream response_stream(&response_);
                         std::string response;
                         std::getline(response_stream, response);
                         if (response != "Hello World!") {
                           parsers_responses.enqueue(response);
                         }
                         start_receive();
                       } else {
                         std::cerr
                             << "PARSER'S CLIENT: Error receiving response: "
                             << ec.message() << std::endl;
                       }
                     });
  }

  void do_connect(const ip::tcp::resolver::results_type &endpoints) {
    async_connect(socket_, endpoints,
                  [this](boost::system::error_code ec, ip::tcp::endpoint) {
                    if (!ec) {
                      if (PARSER_LOGS_ON) {
                        std::cout
                            << "PARSER'S CLIENT: Connected to parser's server."
                            << std::endl;
                      }
                      start();
                    } else {
                      std::cerr
                          << "PARSER'S CLIENT: Error connecting to server: "
                          << ec.message() << std::endl;
                    }
                  });
  }

private:
  boost::asio::io_context &io_context_;
  ip::tcp::socket socket_;
  boost::asio::streambuf response_;
  boost::asio::steady_timer check_timer{io_context_};
};

} // namespace p2p

#endif // PARSER_CONNECTION_CLIENT_HPP_
