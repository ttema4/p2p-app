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

// Запускаем в отдельном потоке из Main::run
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

// Запускаем в отдельном потоке из Main::run
void raise_users_server(uint16_t users_server_port) {
  // Поднимаем сервер для подключений клиентов:
  auto users_server = UsersConnectionsServer();
  users_server.listen(users_server_port);
  users_server.run();
}

struct Main {
  // DataReceiver reciever;
  // Analysis analysis;
  // DataSender sender;

  // Вероятно, оно будет не совсем так, но в упрощенном виде можно представлять
  // что-то такое 
  // void run(){
  //     std::thread t1(&DataReceiver::receive, &reciever);
  //     std::thread t2(&Analysis::analyze, &analysis);
  //     std::thread t3(&DataSender::send, &sender);
  //     std::thread t4(&DataSender::connection_handler, &sender)
  // }
};

} // namespace p2p
