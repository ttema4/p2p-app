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

void fix_banks_naming_and_filter(Orders& orders) {
  Orders new_orders;
  for(const auto& ord : orders.list) {
    std::vector<std::string> new_banks;
    for(std::string bank : ord.banks) {
      if (bank == "A-Bank") {
        new_banks.push_back("Alpha");
      } else if (bank == "Sberbank" || bank == "Local Card(Green)") {
        new_banks.push_back("Sber");
      } else if (bank == "Raiffeisenbank") {
        new_banks.push_back("Raif");
      } else if (bank == "Tinkoff" || bank == "Local Card(Yellow)") {
        new_banks.push_back("Tinkoff");
      } else if (bank == "FPS" || bank == "SBP") {
        new_banks.push_back("SBP");
      }
    }
    if(new_banks.size() > 0){
      Order new_order = ord;
      new_order.banks = new_banks;
      new_orders.list.push_back(new_order);
    }
  }
  orders = new_orders;
}

void unpack_json(std::string parsers_response, Orders& orders, MarketRates& market_rates) {
  nlohmann::json j;
  try {
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

  fix_banks_naming_and_filter(orders);

  for (const auto& rate_obj : j["spot_rates"]) {
      for (auto it = rate_obj.begin(); it != rate_obj.end(); ++it) {
          if (it.key() == "market") continue;

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

std::string pack_json(Chains& chains) {
  nlohmann::json j;
  for (Chain chain : chains.list) {
    nlohmann::json chain_json;
    chain_json["buy"]["market"] = chain.buy.market;
    chain_json["buy"]["type"] = chain.buy.type;
    chain_json["buy"]["id"] = chain.buy.id;
    chain_json["buy"]["seller_rating"] = chain.buy.seller_rating;
    chain_json["buy"]["coin1"] = chain.buy.coin1;
    chain_json["buy"]["coin2"] = chain.buy.coin2;
    chain_json["buy"]["banks"] = chain.buy.banks;
    chain_json["buy"]["min_max"] = chain.buy.min_max;
    chain_json["buy"]["exchange_rate"] = chain.buy.exchange_rate;

    chain_json["change"] = chain.change;

    chain_json["sell"]["market"] = chain.sell.market;
    chain_json["sell"]["type"] = chain.sell.type;
    chain_json["sell"]["id"] = chain.sell.id;
    chain_json["sell"]["seller_rating"] = chain.sell.seller_rating;
    chain_json["sell"]["coin1"] = chain.sell.coin1;
    chain_json["sell"]["coin2"] = chain.sell.coin2;
    chain_json["sell"]["banks"] = chain.sell.banks;
    chain_json["sell"]["min_max"] = chain.sell.min_max;
    chain_json["sell"]["exchange_rate"] = chain.sell.exchange_rate;

    chain_json["spread"] = chain.spread;

    j.push_back(chain_json);
  }
  
  return j.dump();
}

} // namespace p2p

#endif // JSON_TOOLS_HPP_