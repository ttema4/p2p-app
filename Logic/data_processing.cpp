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

// struct DataReceiver {
//   void receive() {}
// };

struct Analysis {
  void analyze(Chains &chains, Orders &orders_for_buy, Orders &orders_for_sell,
               MarketRates &market_rates) {
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
};

// struct DataSender {
//   void send() {}
// };

} // namespace p2p
