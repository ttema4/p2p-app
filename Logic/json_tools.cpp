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

// На данный момент метод unpack_json находится в разработке...
// |
// |
// V
void unpack_json(std::string parsers_response) {
  // nlohmann::json j = nlohmann::json::parse(parsers_response);
  // Orders orders;
  // for (auto &order : j["orders"]) {
  //   orders.list.push_back(Order(order["type"], order["id"],
  //   order["seller_rating"], order["coin1"], order["coin2"], order["banks"],
  //   order["min_max"], order["exchange_rate"]));
  // }
  // MarketRates market_rates;
  // for (auto &market : j["market_rates"]) {
  //   std::vector<std::pair<std::string, long double>> rates;
  //   for (auto &rate : market["rates"]) {
  //     rates.push_back(std::make_pair(rate["coin"], rate["rate"]));
  //   }
  //   market_rates.list[market["coin"]] = rates;
  // }

  // orders_parser_to_analysis.enqueue(orders);
  // market_rates_parser_to_analysis.enqueue(market_rates);
}

// На данный момент метод pack_json находится в разработке...
void pack_json() {
  // Chains chains;
  // while(chains_analysis_to_sender.try_dequeue(chains)){
  //   nlohmann::json j;
  //   for(auto &chain : chains.list){
  //     j.push_back({
  //       {"buy", {
  //         {"type", chain.buy.type},
  //         {"id", chain.buy.id},
  //         {"seller_rating", chain.buy.seller_rating},
  //         {"coin1", chain.buy.coin1},
  //         {"coin2", chain.buy.coin2},
  //         {"bank", chain.buy.bank},
  //         {"min_max", chain.buy.min_max},
  //         {"exchange_rate", chain.buy.exchange_rate}
  //       }},
  //       {"change", {
  //         {"coin1", chain.change.first},
  //         {"coin2", chain.change.second}
  //       }},
  //       {"sell", {
  //         {"type", chain.sell.type},
  //         {"id", chain.sell.id},
  //         {"seller_rating", chain.sell.seller_rating},
  //         {"coin1", chain.sell.coin1},
  //         {"coin2", chain.sell.coin2},
  //         {"bank", chain.sell.bank},
  //         {"min_max", chain.sell.min_max},
  //         {"exchange_rate", chain.sell.exchange_rate}
  //       }},
  //       {"spread", chain.spread}
  //     });
  //   }
  //   // j.dump() - наш апдейт, который теперь при очередном запросе клиента
  //   можем отправить
  // }
}


} // namespace p2p