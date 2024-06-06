#include "scraper.hpp"

void scraper::add_market(std::unique_ptr<market> market_) {
    markets.push_back(std::move(market_));
}

void scraper::update_markets() {
    for (auto &i : markets) {
        i->update_market();
    }
}

void scraper::handle() {
    std::vector<nlohmann::json> json_orders;
    std::vector<nlohmann::json> json_spot_rates;
    int last_size;
    for (auto &i : markets) {
        last_size = json_orders.size();
        json_orders.resize(json_orders.size() + i->json_orders.size());
        for (int j = last_size; j < json_orders.size(); j++) {
            json_orders[j] = i->json_orders[j - last_size];
        }
        json_spot_rates.push_back(i->json_spot_rates);
    }
    std::ofstream data_market_file;
    data_market_file.open("/Users/data_market.json");
    nlohmann::json json_data_market{};
    json_data_market["orders"] = json_orders;
    json_data_market["spot_rates"] = json_spot_rates;
    data_market_file << json_data_market;
    data_market_file.close();
}