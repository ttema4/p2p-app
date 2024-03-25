#ifndef SCRAPER_HPP_
#define SCRAPER_HPP_

#include "exchanges.hpp"
#include "magic_enum/magic_enum.hpp"

struct scraper {
    std::vector<std::unique_ptr<exchange>> exchanges;

  void add_exchange(std::unique_ptr<exchange> exchange_);

  void update_exchanges_orders();

  void handle();
};

#endif