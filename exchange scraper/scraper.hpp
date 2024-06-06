#ifndef SCRAPER_HPP_
#define SCRAPER_HPP_

#include "markets.hpp"

std::vector<std::string> local_bank_is(std::vector<std::string> &payment_methods
);

struct scraper {
    std::vector<std::unique_ptr<market>> markets;

    void add_market(std::unique_ptr<market> market_);

    void update_markets();

    void handle();
};

#endif