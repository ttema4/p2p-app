#include "scraper.hpp"
#include "nlohmann/json.hpp"

void scraper::add_exchange(std::unique_ptr<exchange> exchange_) {
  exchanges.push_back(std::move(exchange_));
}

void scraper::update_exchanges_orders() {
  for (auto &i : exchanges) {
    i->update_exchange_orders();
  }
}

void scraper::handle() {
  std::vector<nlohmann::json> json_orders_;
  nlohmann::json json_order{};
  for (auto &i : exchanges) {
    for (auto &j : i->orders) {
      json_order["exchange"] = magic_enum::enum_name(j->exchange);
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