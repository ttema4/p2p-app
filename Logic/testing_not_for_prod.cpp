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
    
// ДАЛЕЕ КОД ДЛЯ ТЕСТИРОВАНИЯ

// Если на этапе этого коммита хочется увидеть, как функционируют сервер и
// клиент, то можно запустить следующий main. Примечание 1: отдельно должен быть
// запущен прототип сервера парсера. Примечание 3: клиентов для сервера(так
// называемых пользователей) также нужно запустить отдельно(тут в принципе
// прокатит и netcat). Примечание 3: в данном коде используются временные
// параметры, которые в дальнейшем будут заменены на конфигурационные.

int main() {
  std::thread t1(&raise_parser_connection, "localhost",
                 "12345"); // Тут указанные временные параметры
  std::thread t2(&raise_users_server, 12346); // Тут указаны временные параметры
  t1.join();
  t2.join();
}

} // namespace p2p