#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include <queue>
#include <map>
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

  // std::cout << "orders_for_sell size: " << orders_for_sell.list.size() << std::endl;
  // for(int i = 0; i < orders_for_sell.list.size(); i++){
  //   std::cout << i << ". Продаём: " << orders_for_sell.list[i].coin2 << " за " << orders_for_sell.list[i].coin1 << " по цене " << orders_for_sell.list[i].exchange_rate << std::endl;
  // }
  // std::cout << "orders_for_buy size: " << orders_for_buy.list.size() << std::endl;
  // for(int i = 0; i < 5; i++){
  //   std::cout << i << ". Покупаем: " << orders_for_buy.list[i].coin2 << " за " << orders_for_buy.list[i].coin1 << " по цене " << orders_for_buy.list[i].exchange_rate << std::endl;
  // }
  // std::cout << "Текущие курсы обмена: " << std::endl;
  // for(auto it = market_rates.list.begin(); it != market_rates.list.end(); ++it){
  //   for(auto it2 = it->second.begin(); it2 != it->second.end(); ++it2){
  //     std::cout << it->first << " " << it2->first << " " << it2->second << std::endl;
  //   }
  // }

  std::unordered_map<std::string, std::vector<Order>> sell_by_coin;
  for (Order sell : orders_for_sell.list) {
    // std::cout << "SELLING: " << sell.coin2 << std::endl;
    sell_by_coin[sell.coin2].push_back(sell);
  }

  // std::cout << "sell_by_coin size: " << sell_by_coin.size() << std::endl;
  // std::cout << "sell_by_coin" << std::endl;
  // for(auto it = sell_by_coin.begin(); it != sell_by_coin.end(); ++it){
  //   std::cout << it->first << std::endl;
  // }
  // std::cout << "market_rates:" << std::endl;
  // for(auto it = market_rates.list.begin(); it != market_rates.list.end(); ++it){
  //   for(auto it2 = it->second.begin(); it2 != it->second.end(); ++it2){
  //     std::cout << it->first << " " << it2->first << " " << it2->second << std::endl;
  //   }
  // }

  std::unordered_map<std::string, Chain> chains_map;
  for(Order buy : orders_for_buy.list){
    long double current_sum_1 = 1000;
    std::string coin_buy = buy.coin2;
    current_sum_1 = current_sum_1 / buy.exchange_rate;
    // std::cout << "Покупаем " << coin_buy << " по курсу " << buy.exchange_rate << " 1000 -> " << current_sum_1 << std::endl;
    for(std::pair<std::string, long double> market : market_rates.list[coin_buy]){
      long double current_sum_2 = current_sum_1;
      std::string coin_sell = market.first;
      long double rate = market.second;
      current_sum_2 = current_sum_2 * rate;
      // std::cout << "Меняем " << coin_buy << " на " << coin_sell << " по курсу " << rate << " " << current_sum_2 << std::endl;
      for(Order sell : sell_by_coin[coin_sell]){
        long double current_sum_3 = current_sum_2;
        current_sum_3 = current_sum_3 * sell.exchange_rate;
        long double spread = (current_sum_3 / 1000 - 1) * 100;
        // std::cout << "Spread: " << spread << std::endl;
        Chain chain = Chain(buy, std::make_pair(coin_buy, coin_sell), sell, spread);
        std::string key = coin_buy + "-" + coin_sell;
        // std::cout << "Key: " << key << std::endl;
        if(chains_map.find(key) == chains_map.end()){
          chains_map[key] = chain;
        } else {
          if(chains_map[key].spread < spread){
            chains_map[key] = chain;
          }
        }
      }
    }
  }

  chains.list.clear();
  for(auto it = chains_map.begin(); it != chains_map.end(); ++it){
    chains.list.push_back(it->second);
  }
}

// struct DataSender {
//   void send() {}
// };

} // namespace p2p
