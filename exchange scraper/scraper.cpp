#include "scraper.hpp"

void scraper::add_market(std::unique_ptr<market> market_) {
    markets.push_back(std::move(market_));
}

void scraper::update_markets() {
    for (auto &i : markets) {
        i->update_market_orders();
        i->update_spot_rates();
    }
}

std::vector<std::string> local_bank_is(std::vector<std::string> &payment_methods
) {
    bool wasSberbank = false;
    bool wasA_bank = false;
    std::vector<std::string> payment_methods_;
    for (auto i : payment_methods) {
        if (i == "Local Card(Yellow)") {
            payment_methods_.push_back("Tinkoff");
        } else if (i == "Local Card(R-Green)" || i == "Local Card(S-Green)" || i == "Local Card(Green)") {
            if (!wasSberbank) {
                payment_methods_.push_back("Sberbank");
                wasSberbank = true;
            }
        } else if (i == "Local Card(Red)") {
            if (!wasA_bank) {
                payment_methods_.push_back("A-Bank");
                wasA_bank = true;
            }
        } else {
            if (i == "A-Bank") {
                if (!wasA_bank) {
                    payment_methods_.push_back(i);
                    wasA_bank = true;
                }
            } else {
                payment_methods_.push_back(i);
            }
        }
    }
    return payment_methods_;
}

void scraper::handle() {
    std::vector<nlohmann::json> json_orders;
    nlohmann::json json_order{};
    for (auto &i : markets) {
        for (auto &j : i->orders) {
            json_order["market"] = magic_enum::enum_name(i->market_);
            json_order["currency"] = magic_enum::enum_name(j->currency);
            json_order["coin"] = magic_enum::enum_name(j->coin);
            json_order["direction"] = magic_enum::enum_name(j->direction);
            json_order["price"] = decimal2_to_string(j->price);
            json_order["payment_methods"] = local_bank_is(j->payment_methods);
            json_order["lower_limit"] = decimal2_to_string(j->lower_limit);
            json_order["upper_limit"] = decimal2frp_to_string(j->upper_limit);
            json_order["available"] = decimal8_to_string(j->available);
            json_order["link"] = j->link;
            json_orders.push_back(json_order);
            json_order.erase(json_order.begin(), json_order.end());
        }
    }
    std::vector<nlohmann::json> json_spot_rates;
    nlohmann::json json_spot_rates_{};
    for (auto &i : markets) {
        json_spot_rates_["market"] = magic_enum::enum_name(i->market_);
        for (int j = 0; j < 4; ++j) {
            for (int k = j + 1; k < 4; ++k) {
                switch (coins[j]) {
                    case Coins::USDT:
                        switch (coins[k]) {
                            case Coins::USDC:
                                json_spot_rates_["USDC/USDT"] =
                                    decimal8_to_string(
                                        i->spot_rates[coins[j]][coins[k]]
                                    );
                                break;

                            case Coins::BTC:
                                json_spot_rates_["BTC/USDT"] =
                                    decimal8_to_string(
                                        i->spot_rates[coins[j]][coins[k]]
                                    );
                                break;

                            case Coins::ETH:
                                json_spot_rates_["ETH/USDT"] =
                                    decimal8_to_string(
                                        i->spot_rates[coins[j]][coins[k]]
                                    );
                                break;
                        }
                        break;

                    case Coins::USDC:
                        switch (coins[k]) {
                            case Coins::BTC:
                                json_spot_rates_["BTC/USDC"] =
                                    decimal8_to_string(
                                        i->spot_rates[coins[j]][coins[k]]
                                    );
                                break;

                            case Coins::ETH:
                                json_spot_rates_["ETH/USDC"] =
                                    decimal8_to_string(
                                        i->spot_rates[coins[j]][coins[k]]
                                    );
                                break;
                        }
                        break;

                    case Coins::BTC:
                        json_spot_rates_["ETH/BTC"] =
                            decimal8_to_string(i->spot_rates[coins[j]][coins[k]]
                            );
                        break;
                }
            }
        }
        json_spot_rates.push_back(json_spot_rates_);
        json_spot_rates_.erase(
            json_spot_rates_.begin(), json_spot_rates_.end()
        );
    }
    std::ofstream data_market_file;
    data_market_file.open("data_market.json");
    nlohmann::json json_data_market{};
    json_data_market["orders"] = json_orders;
    json_data_market["spot_rates"] = json_spot_rates;
    data_market_file << json_data_market;
    data_market_file.close();
}