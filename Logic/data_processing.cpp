#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "storage_structures.hpp"
#include "data_processing.hpp"

// #include "concurrentqueue.h"

namespace p2p {

// struct DataReceiver {
//   void receive() {}
// };

void Analysis::analyze(Chains &chains, Orders &orders, MarketRates &market_rates) {
  Orders orders_for_sell;
  Orders orders_for_buy;
  for (Order order : orders.list) {
    if (order.type == "sell") {
      orders_for_sell.list.push_back(order);
    } else if (order.type == "buy") {
      orders_for_buy.list.push_back(order);
    }
  }
  std::unordered_map<std::string, std::vector<Order>> sell_by_coin;
  for (Order sell : orders_for_sell.list) {
    sell_by_coin[sell.coin1].push_back(sell);
  }
  for (Order buy : orders_for_buy.list) {
    std::string coin_buy = buy.coin2;
    for (std::pair<std::string, long double> market :
          market_rates.list[coin_buy]) {
      std::string coin_sell = market.first;
      long double rate = market.second;
      for (Order sell : sell_by_coin[coin_sell]) {
        long double spread =
            ((sell.exchange_rate / buy.exchange_rate) - 1) * 100;
        chains.list.push_back(
            Chain(buy, std::make_pair(coin_buy, coin_sell), sell, spread));
      }
    }
  }
}

// struct DataSender {
//   void send() {}
// };

} // namespace p2p
