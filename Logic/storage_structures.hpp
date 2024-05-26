#ifndef STORAGE_STRUCTURES_HPP_
#define STORAGE_STRUCTURES_HPP_

#include <cstdint>
#include <iostream>
#include <list>
#include <memory>
#include <shared_mutex>
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
  Order(std::string market_, std::string type_, std::string id_,
        long double seller_rating_, std::string coin1_, std::string coin2_,
        std::vector<std::string> banks_,
        std::pair<long double, long double> min_max_,
        long double exchange_rate_)
      : market(market_), type(type_), id(id_), seller_rating(seller_rating_),
        coin1(coin1_), coin2(coin2_), banks(banks_), min_max(min_max_),
        exchange_rate(exchange_rate_) {}
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

  Chain() = default;
  Chain(Order buy_, const std::pair<std::string, std::string> change_,
        Order sell_, long double spread_)
      : buy(buy_), change(change_), sell(sell_), spread(spread_) {}
};

struct Chains {
  std::vector<Chain> list;
  Chains() = default;
};

extern moodycamel::ConcurrentQueue<std::string> parsers_responses;

class SharedString {
private:
  std::string str;
  mutable std::shared_mutex mtx;

public:
  SharedString() = default;
  SharedString(const std::string &initial_str) : str(initial_str) {}

  std::string get() const {
    std::shared_lock lock(mtx);
    return str;
  }

  void set(const std::string &new_str) {
    std::unique_lock lock(mtx);
    str = new_str;
  }
};

extern SharedString up_to_date_version;
extern std::unordered_map<std::string, std::string> last_version_by_user;

extern std::string PARSER_IP;
extern uint16_t PARSER_PORT;
extern uint16_t USERS_SERVER_PORT;
extern bool PARSER_LOGS_ON;
extern bool PARSER_RESPONSE_LOGS_ON;
extern bool USERS_SERVER_LOGS_ON;
extern bool ORDERS_FOR_BUY_LOGS_ON;
extern bool ORDERS_FOR_SELL_LOGS_ON;
extern bool SELL_BY_COIN_LOGS_ON;
extern bool MARKET_RATES_LOGS_ON;
extern bool CHAINS_LOGS_ON;

} // namespace p2p

#endif // STORAGE_STRUCTURES_HPP_