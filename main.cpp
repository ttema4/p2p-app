#include <iostream>
#include <memory>
#include <string>
#include <vector>

enum class Currencies {};

enum class Coins {};

enum class Directions { currency_coin, coin_currency };

struct order {
  Currencies currency;
  Coins coin;
  Directions direction;
  int price;
  std::string payment_method;
  int lower_limit;
  int upper_limit;
  int available;
  std::string link;
};

struct exchange {};

struct exchange_simulator : exchange {};

struct scraper {
  std::vector<std::unique_ptr<exchange>> exchanges;

  void add_exchange(std::unique_ptr<exchange> exchange_) {
    exchanges.push_back(std::make_unique<exchange>());
    exchanges[exchanges.size() - 1] = std::move(exchange_);
  }

  void handle() {}
};