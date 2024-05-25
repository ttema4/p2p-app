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

#include "storage_structures.hpp"
#include "parser_connection_client.hpp"
#include "users_connections_server.hpp"
#include "data_processing.hpp"
#include "json_tools.hpp"


using namespace boost::asio;

moodycamel::ConcurrentQueue<std::string> p2p::parsers_responses;
p2p::SharedString p2p::up_to_date_version;

std::string p2p::PARSER_IP;
uint16_t p2p::PARSER_PORT;
uint16_t p2p::USERS_SERVER_PORT;
bool p2p::PARSER_LOGS_ON;
bool p2p::PARSER_RESPONSE_LOGS_ON;
bool p2p::USERS_SERVER_LOGS_ON;
bool p2p::ORDERS_FOR_BUY_LOGS_ON;
bool p2p::ORDERS_FOR_SELL_LOGS_ON;
bool p2p::SELL_BY_COIN_LOGS_ON;
bool p2p::MARKET_RATES_LOGS_ON;
bool p2p::CHAINS_LOGS_ON;

namespace p2p {

void raise_parser_connection_and_users_server() {
  boost::asio::io_context global_io_context;
  ip::tcp::resolver resolver(global_io_context);
  auto endpoints = resolver.resolve(PARSER_IP, std::to_string(PARSER_PORT));
  ParserConnectionClient client(global_io_context, endpoints);
  auto users_server = UsersConnectionsServer(global_io_context);
  users_server.listen(USERS_SERVER_PORT);
  global_io_context.run();
}

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
      if(PARSER_RESPONSE_LOGS_ON){
        std::cout << "Last response: " << last_response << std::endl;
      }

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

      if(CHAINS_LOGS_ON){
        std::cout << "Chains size: " << chains.list.size() << std::endl;
        std::cout << "Chains:" << std::endl;
        for(auto &chain : chains.list){
          std::cout << "Buy: " << chain.buy.coin2 << ", " << chain.change.first << " -> " << chain.change.second<< ", Sell: " << chain.sell.coin2 << ", Spread: " << chain.spread << "%" << std::endl;
        }
      }

      up_to_date_version.set(pack_json(chains));

    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Пауза на 100 миллисекунд
    }
  }
}

void apply_config(){
  std::ifstream config("config.json");
  nlohmann::json j;
  config >> j;
  config.close();
  PARSER_IP = j["endpoints"]["parser's ip"].get<std::string>();
  PARSER_PORT = j["endpoints"]["parser's port"].get<uint16_t>();
  USERS_SERVER_PORT = j["endpoints"]["users server's port"].get<uint16_t>();
  PARSER_LOGS_ON = j["logs to output"]["parser's client"].get<bool>();
  PARSER_RESPONSE_LOGS_ON = j["logs to output"]["parser's server last response"].get<bool>();
  USERS_SERVER_LOGS_ON = j["logs to output"]["users server"].get<bool>();
  ORDERS_FOR_BUY_LOGS_ON = j["logs to output"]["orders for buy"].get<bool>();
  ORDERS_FOR_SELL_LOGS_ON = j["logs to output"]["orders for sell"].get<bool>();
  SELL_BY_COIN_LOGS_ON = j["logs to output"]["sell by coin"].get<bool>();
  MARKET_RATES_LOGS_ON = j["logs to output"]["market rates"].get<bool>();
  CHAINS_LOGS_ON = j["logs to output"]["chains"].get<bool>();

}

} // namespace p2p

int main(){
  p2p::apply_config();
  std::thread t1(&p2p::raise_parser_connection_and_users_server);
  std::thread t2(&p2p::to_analysis);
  t1.join();
  t2.join();
}