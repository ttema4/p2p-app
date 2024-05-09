#ifndef JSON_TOOLS_HPP_
#define JSON_TOOLS_HPP_

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "storage_structures.hpp"
#include "include/nlohmann/json.hpp"
// #include "concurrentqueue.h"

namespace p2p {

// На данный момент метод unpack_json находится в разработке...
// |
// |
// V
void unpack_json(std::string parsers_response, Orders& orders, MarketRates& market_rates) {
  nlohmann::json j;
  try {
    // std::cout << "parsers_response: " << parsers_response.substr(0, 25) << std::endl;
    j = nlohmann::json::parse(parsers_response);
  } catch (const nlohmann::json::parse_error& e) {
    std::cout << "JSON parse error(inside unpack()): " << e.what() << '\n';
    return;
  }
  for (const auto& order : j["orders"]) {
    Order ord;
    ord.market = order["market"].get<std::string>();
    ord.type = order["direction"].get<std::string>();
    ord.id = "-";
    ord.seller_rating = 0.0;
    ord.coin1 = order["currency"].get<std::string>();
    ord.coin2 = order["coin"].get<std::string>();
    ord.min_max = std::make_pair(std::stold(order["lower_limit"].get<std::string>()), std::stold(order["upper_limit"].get<std::string>()));
    ord.exchange_rate = std::stold(order["price"].get<std::string>());
    for (const auto& bank : order["payment_methods"]) {
        ord.banks.push_back(bank.get<std::string>());
    }
    orders.list.push_back(ord);
  }

  for (const auto& rate_obj : j["spot_rates"]) {
      for (auto it = rate_obj.begin(); it != rate_obj.end(); ++it) {
          if (it.key() == "market") continue; // пропустить поле market

          std::string coins_pair = it.key();
          size_t delimiter_pos = coins_pair.find('/');
          std::string coin1 = coins_pair.substr(0, delimiter_pos);
          std::string coin2 = coins_pair.substr(delimiter_pos + 1);
          long double rate12 = std::stold(it.value().get<std::string>());
          long double rate21 = 1 / rate12;
          market_rates.list[coin1].push_back(std::make_pair(coin2, rate12));
          market_rates.list[coin2].push_back(std::make_pair(coin1, rate21));
      }
  }
  
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

#endif // JSON_TOOLS_HPP_