#include "scraper.hpp"

void scraper::add_market(std::unique_ptr<market> market_) {
    markets.push_back(std::move(market_));
}

void scraper::update_markets_orders() {
    for (auto &i : markets) {
        i->update_market_orders();
    }
}

void scraper::handle() {
    std::vector<nlohmann::json> json_orders_;
    nlohmann::json json_order{};
    for (auto &i : markets) {
        for (auto &j : i->orders) {
            json_order["market"] = magic_enum::enum_name(j->market);
            json_order["currency"] = magic_enum::enum_name(j->currency);
            json_order["coin"] = magic_enum::enum_name(j->coin);
            json_order["direction"] = magic_enum::enum_name(j->direction);
            json_order["price"] = decimal2_to_string(j->price);
            json_order["payment_methods"] = j->payment_methods;
            json_order["lower_limit"] = decimal2_to_string(j->lower_limit);
            json_order["upper_limit"] = decimal2frp_to_string(j->upper_limit);
            json_order["available"] = decimal8_to_string(j->available);
            json_order["link"] = j->link;
            json_orders_.push_back(json_order);
            json_order.erase(json_order.begin(), json_order.end());
        }
    }
    std::ofstream file_of_orders;
    file_of_orders.open("orders.json");
    nlohmann::json json_orders{};
    json_orders["orders"] = json_orders_;
    file_of_orders << json_orders;
    file_of_orders.close();
}