#define BOOST_BIND_GLOBAL_PLACEHOLDERS

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

#include "storage_structures.hpp"
#include "parser_connection_client.hpp"
#include "users_connections_server.hpp"
#include "data_processing.hpp"
#include "json_tools.hpp"


using namespace boost::asio;
moodycamel::ConcurrentQueue<std::string> p2p::parsers_responses;

namespace p2p {

void raise_parser_connection_and_users_server(std::string parser_connection_ip,
                              uint16_t parser_connection_port, uint16_t users_server_port) {
  boost::asio::io_context global_io_context;
  ip::tcp::resolver resolver(global_io_context);
  auto endpoints = resolver.resolve(parser_connection_ip, std::to_string(parser_connection_port));
  ParserConnectionClient client(global_io_context, endpoints);
  auto users_server = UsersConnectionsServer(global_io_context);
  users_server.listen(users_server_port);
  global_io_context.run();
}

struct Main {

  void run(){
    raise_parser_connection_and_users_server("localhost", 12345, 12346);
  }
  
};

void to_analysis(){
  std::string response;
  std::string last_response;
  while(true){
    bool has_new_responses = false;
    while(parsers_responses.try_dequeue(response)){
      last_response = response;
      has_new_responses = true;
    }
    if(has_new_responses){
      // std::cout << "Last response: " << last_response.substr(0, 25) << std::endl;

      Orders orders;
      MarketRates market_rates;
      if (!last_response.empty() && last_response[0] == '{') {
          unpack_json(last_response, orders, market_rates);
      } else {
          std::cout << "Received empty response.\n";
          continue;
      }
      Analysis analysis;
      Chains chains;
      analysis.analyze(chains, orders, market_rates);

      // std::cout << "Chains size: " << chains.list.size() << std::endl;
      // std::cout << "Chains:" << std::endl;
      // for(auto &chain : chains.list){
      //   std::cout << "Buy: " << chain.buy.coin2 << ", " << chain.change.first << " -> " << chain.change.second<< ", Sell: " << chain.sell.coin2 << ", Spread: " << chain.spread << "%" << std::endl;
      // }

    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Пауза на 100 миллисекунд
    }
  }
}

} // namespace p2p

int main(){
  p2p::Main mainObject;
  std::thread t1(&p2p::Main::run, &mainObject);
  std::thread t2(&p2p::to_analysis);
  t1.join();
  t2.join();
}