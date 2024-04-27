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
    check_timer.expires_after(
        std::chrono::seconds(1)); // Проверяем обновления каждую секунду
    check_timer.async_wait([this](boost::system::error_code ec) {
      if (!ec) {
        send_request(
            "need update"); // Запрос, по которому парсер присылает обновление,
                            // либо говорит о его отсутствии
        start_check_updates();
      }
    });
  }

  void send_request(const std::string &request) {
    std::cout << "Sending request: " << request << std::endl; // Для дебага
    async_write(socket_, buffer(request + '\n'),
                [this](boost::system::error_code ec, std::size_t /*length*/) {
                  if (ec) {
                    std::cerr << "Error sending request: " << ec.message()
                              << std::endl; // Возможно, здесь будет более умная
                                            // обработка ошибок
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
                         std::cout << "Received response: " << response
                                   << std::endl; // Для дебага
                         // |
                         // |
                         // V
                         // response - наши данные, которые мы получили
                         // Здесь кладем в очередь / передаем во внешний метод -
                         // этот этап в работе... Скорее всего, хотим сначала
                         // прямо здесь проверить, получили ли вообще апдейт
                         // Callback на обработку данных??? или прямой вызов
                         // unpack_json прямо в этом потоке?
                         start_receive();
                       } else {
                         std::cerr
                             << "Error receiving response: " << ec.message()
                             << std::endl; // Возможно, здесь будет более умная
                                           // обработка ошибок
                       }
                     });
  }

  void do_connect(const ip::tcp::resolver::results_type &endpoints) {
    async_connect(socket_, endpoints,
                  [this](boost::system::error_code ec, ip::tcp::endpoint) {
                    if (!ec) {
                      std::cout << "Connected to server."
                                << std::endl; // Для дебага
                      start();
                    } else {
                      std::cerr
                          << "Error connecting to server: " << ec.message()
                          << std::endl; // Возможно, здесь будет более умная
                                        // обработка ошибок
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
