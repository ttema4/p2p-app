#ifndef SCRAPER_HPP_
#define SCRAPER_HPP_

#include <fstream>
#include "markets.hpp"
#include "magic_enum/magic_enum.hpp"
#include "nlohmann/json.hpp"

struct scraper {
    std::vector<std::unique_ptr<market>> markets;

    void add_market(std::unique_ptr<market> market_);

    void update_markets_orders();

    void handle();
};

#endif