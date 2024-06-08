#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
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

#include "include/concurrentqueue.h"
#include "parser_connection_client.hpp"
#include "storage_structures.hpp"
#include "users_connections_server.hpp"

using namespace boost::asio;

namespace p2p {

// ДАЛЕЕ КОД ДЛЯ ТЕСТИРОВАНИЯ

// Если на этапе этого коммита хочется увидеть, как функционируют сервер и
// клиент, то можно запустить следующий main. Примечание 1: отдельно должен быть
// запущен прототип сервера парсера. Примечание 3: клиентов для сервера(так
// называемых пользователей) также нужно запустить отдельно(тут в принципе
// прокатит и netcat). Примечание 3: в данном коде используются временные
// параметры, которые в дальнейшем будут заменены на конфигурационные.

void raise_parser_connection(std::string parser_connection_ip,
                             std::string parser_connection_port) {
  // Поднимаем клиента для подключения к парсеру:
  boost::asio::io_context client_io_context;
  ip::tcp::resolver resolver(client_io_context);
  auto endpoints =
      resolver.resolve(parser_connection_ip, parser_connection_port);
  ParserConnectionClient client(client_io_context, endpoints);
  client_io_context.run();
}

void raise_users_server(uint16_t users_server_port) {
  // Поднимаем сервер для подключений клиентов:
  auto users_server = UsersConnectionsServer();
  users_server.listen(users_server_port);
  users_server.run();
}

} // namespace p2p

int main() {
  std::thread t1(&p2p::raise_parser_connection, "localhost",
                 "12345"); // Тут указанные временные параметры
  std::thread t2(&p2p::raise_users_server,
                 12346); // Тут указаны временные параметры
  t1.join();
  t2.join();
}
