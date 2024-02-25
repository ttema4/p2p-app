#include "exchanges.hpp"

struct scraper {
  std::vector<std::unique_ptr<exchange>> exchanges;

  void add_exchange(exchange *exchange_);
};