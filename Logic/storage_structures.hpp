#ifndef STORAGE_STRUCTURES_HPP_
#define STORAGE_STRUCTURES_HPP_

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "include/concurrentqueue.h"

namespace p2p {

struct Order {
  std::string market;
  std::string type;
  std::string id;
  long double seller_rating;
  std::string coin1;
  std::string coin2;
  std::vector<std::string> banks;
  std::pair<long double, long double> min_max;
  long double exchange_rate;
  Order() = default;
  Order(std::string market_, std::string type_, std::string id_, long double seller_rating_,
        std::string coin1_, std::string coin2_, std::vector<std::string> banks_,
        std::pair<long double, long double> min_max_, long double exchange_rate_)
      : market(market_), type(type_), id(id_), seller_rating(seller_rating_), coin1(coin1_),
        coin2(coin2_), banks(banks_), min_max(min_max_),
        exchange_rate(exchange_rate_) {}

  // В разработке
  // NLOHMANN_DEFINE_TYPE_INTRUSIVE(Order, market, type, id, seller_rating, coin1,
  // coin2, banks, min_max, exchange_rate);
};

struct Orders {
  std::vector<Order> list;
  Orders() = default;
};

struct MarketRates {
  std::unordered_map<std::string,
                     std::vector<std::pair<std::string, long double>>>
      list;
  MarketRates() = default;
};

struct Chain {
  Order buy;
  std::pair<std::string, std::string> change;
  Order sell;
  long double spread;

  Chain(Order buy_, const std::pair<std::string, std::string> change_,
        Order sell_, long double spread_)
      : buy(buy_), change(change_), sell(sell_), spread(spread_) {}

  // В разработке
  // NLOHMANN_DEFINE_TYPE_INTRUSIVE(Chain, buy, change, sell, spread);
};

struct Chains {
  std::vector<Chain> list;
  Chains() = default;
};

extern moodycamel::ConcurrentQueue<std::string> parsers_responses;

// В разработке...
// moodycamel::ConcurrentQueue<Orders> orders_parser_to_analysis;
// moodycamel::ConcurrentQueue<MarketRates> market_rates_parser_to_analysis;
// moodycamel::ConcurrentQueue<Chains> chains_analysis_to_sender;

} // namespace p2p 

#endif // STORAGE_STRUCTURES_HPP_