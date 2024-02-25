#include "scraper.hpp"

void scraper::add_exchange(exchange *exchange_) {
  exchanges.push_back(
      std::move(std::unique_ptr<exchange>(std::move(exchange_))));
}