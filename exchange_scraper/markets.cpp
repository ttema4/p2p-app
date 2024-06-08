#include "markets.hpp"
#include <boost/asio.hpp>
#include <cmath>
#include <sstream>
#include "magic_enum/magic_enum.hpp"

order::order(
    Currencies currency_,
    Coins coin_,
    Directions direction_,
    dec::decimal<2> price_,
    std::vector<std::string> &payment_methods_,
    dec::decimal<2> lower_limit_,
    dec::decimal<2, dec::floor_round_policy> upper_limit_,
    dec::decimal<8> available_,
    std::string &link_
)
    : currency(currency_),
      coin(coin_),
      direction(direction_),
      price(price_),
      payment_methods(payment_methods_),
      lower_limit(lower_limit_),
      upper_limit(upper_limit_),
      available(available_),
      link(link_) {
}

bybit_simulator::bybit_simulator() {
    market_ = Markets::bybit_simulator;
    market_payment_methods = {
        "Local Card(Yellow)",
        "Raiffeisenbank",
        "A-Bank",
        "Advcash",
        "BCS bank",
        "Bank Transfer",
        "CitiBank (Russia)",
        "FPS",
        "KoronaPay",
        "Local Card(R-Green)",
        "Local Card(S-Green)",
        "Local Card(Green)",
        "Local Card(Red)",
        "MIR",
        "OTP Bank",
        "OZON Bank",
        "Payeer",
        "QIWI",
        "RSK Bank",
        "Renaissance Credit Bank",
        "Russian Standart Bank",
        "SBP",
        "UniCredit"};
}

std::string decimal2_to_string(dec::decimal<2> n) {
    std::stringstream ss;
    ss << n;
    std::string str_n;
    ss >> str_n;
    return str_n;
}

std::string decimal2frp_to_string(dec::decimal<2, dec::floor_round_policy> n) {
    std::stringstream ss;
    ss << n;
    std::string str_n;
    ss >> str_n;
    return str_n;
}

std::string decimal8_to_string(dec::decimal<8> n) {
    std::stringstream ss;
    ss << n;
    std::string str_n;
    ss >> str_n;
    return str_n;
}

void bybit_simulator::update_market_orders() {
    orders.clear();
    Currencies currencies_[]{Currencies::RUB};
    Coins coins_[]{Coins::BTC, Coins::ETH, Coins::USDC, Coins::USDT};
    Directions directions_[]{Directions::buy, Directions::sell};
    dec::decimal<2> price;
    dec::decimal<2> average_price;
    std::vector<std::string> payment_methods;
    int number_of_banks;
    dec::decimal<2> lower_limit;
    dec::decimal<2, dec::floor_round_policy> upper_limit;
    dec::decimal<8> available;
    dec::decimal<8> min_available;
    std::string link = "";
    int n;
    for (int i = 0; i < 1052; ++i) {
        n = gen() % 32767;
        Currencies currency = currencies_[n % 1];
        Coins coin = coins_[n % 4];
        Directions direction = directions_[n % 2];
        payment_methods.clear();
        switch (currency) {
            case Currencies::RUB:
                switch (coin) {
                    case Coins::BTC:
                        switch (direction) {
                            case Directions::buy:
                                average_price = 4960000;
                                price = average_price +
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 101) / 100)
                                        )) +
                                        (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                            (n % 70001);
                                number_of_banks = n % 6;
                                if (number_of_banks == 0) {
                                    number_of_banks = 1;
                                }
                                for (int j = 0, k = 13; j < number_of_banks;
                                     ++j, ++k) {
                                    payment_methods.push_back(
                                        market_payment_methods[n % k]
                                    );
                                }
                                min_available = dec::decimal_cast<8>("0.00013");
                                available =
                                    min_available +
                                    dec::decimal_cast<8>(std::to_string(
                                        ((double)(n % 49988) / 100000)
                                    )) +
                                    dec::decimal_cast<8>(std::to_string(
                                        ((double)(n % 1000000001) / 1000000000)
                                    ));
                                upper_limit =
                                    dec::decimal_cast<8>(price) * available;
                                if (available > "0.00195") {
                                    upper_limit *=
                                        dec::decimal_cast<2>(std::to_string(
                                            (double)(n % 71) / 100
                                        )) +
                                        "0.3";
                                }
                                lower_limit =
                                    500 + (n % std::stoi(decimal2frp_to_string(
                                                   upper_limit - 500
                                               )));
                                lower_limit += std::to_string(
                                    (double)((n + 313) % 101) / 100
                                );
                                break;

                            case Directions::sell:
                                average_price = 4840000;
                                price = average_price +
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 101) / 100)
                                        )) +
                                        (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                            (n % 30001);
                                number_of_banks = n % 6;
                                if (number_of_banks == 0) {
                                    number_of_banks = 1;
                                }
                                for (int j = 0, k = 13; j < number_of_banks;
                                     ++j, ++k) {
                                    payment_methods.push_back(
                                        market_payment_methods[n % k]
                                    );
                                }
                                min_available = dec::decimal_cast<8>("0.00013");
                                available =
                                    min_available +
                                    dec::decimal_cast<8>(std::to_string(
                                        ((double)(n % 49988) / 100000)
                                    )) +
                                    dec::decimal_cast<8>(std::to_string(
                                        ((double)(n % 1000000001) / 1000000000)
                                    ));
                                upper_limit =
                                    dec::decimal_cast<8>(price) * available;
                                if (available > "0.00195") {
                                    upper_limit *=
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 71) / 100)
                                        )) +
                                        "0.3";
                                }
                                lower_limit =
                                    500 + (n % std::stoi(decimal2frp_to_string(
                                                   upper_limit - 500
                                               )));
                                lower_limit += std::to_string(
                                    (double)((n + 313) % 101) / 100
                                );
                                break;
                        }
                        break;

                    case Coins::ETH:
                        switch (direction) {
                            case Directions::buy:
                                average_price = 294000;
                                price = average_price +
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 101) / 100)
                                        )) +
                                        (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                            (n % 6001);
                                number_of_banks = n % 6;
                                if (number_of_banks == 0) {
                                    number_of_banks = 1;
                                }
                                for (int j = 0, k = 13; j < number_of_banks;
                                     ++j, ++k) {
                                    payment_methods.push_back(
                                        market_payment_methods[n % k]
                                    );
                                }
                                min_available =
                                    dec::decimal_cast<8>("0.00200000");
                                available =
                                    min_available + n % 11 +
                                    dec::decimal_cast<8>(std::to_string(
                                        ((double)(n % 1000000001) / 1000000000)
                                    ));
                                upper_limit =
                                    dec::decimal_cast<8>(price) * available;
                                if (available > "0.3") {
                                    upper_limit *=
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 71) / 100)
                                        )) +
                                        "0.3";
                                }
                                lower_limit =
                                    500 + (n % std::stoi(decimal2frp_to_string(
                                                   upper_limit - 500
                                               )));
                                lower_limit += std::to_string(
                                    (double)((n + 313) % 101) / 100
                                );
                                break;

                            case Directions::sell:
                                average_price = 283000;
                                price = average_price +
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 101) / 100)
                                        )) +
                                        (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                            (n % 3001);
                                number_of_banks = n % 6;
                                if (number_of_banks == 0) {
                                    number_of_banks = 1;
                                }
                                for (int j = 0, k = 13; j < number_of_banks;
                                     ++j, ++k) {
                                    payment_methods.push_back(
                                        market_payment_methods[n % k]
                                    );
                                }
                                min_available =
                                    dec::decimal_cast<8>("0.00200000");
                                available =
                                    min_available + n % 11 +
                                    dec::decimal_cast<8>(std::to_string(
                                        ((double)(n % 1000000001) / 1000000000)
                                    ));
                                upper_limit =
                                    dec::decimal_cast<8>(price) * available;
                                if (available > "0.3") {
                                    upper_limit *=
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 71) / 100)
                                        )) +
                                        "0.3";
                                }
                                lower_limit =
                                    500 + (n % std::stoi(decimal2frp_to_string(
                                                   upper_limit - 500
                                               )));
                                lower_limit += std::to_string(
                                    (double)((n + 313) % 101) / 100
                                );
                                break;
                        }
                        break;

                    case Coins::USDC:
                        switch (direction) {
                            case Directions::buy:
                                average_price = 98.50;
                                price =
                                    average_price +
                                    std::to_string(
                                        (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                        ((double)(n % 301) / 100)
                                    );
                                number_of_banks = n % 6;
                                if (number_of_banks == 0) {
                                    number_of_banks = 1;
                                }
                                for (int j = 0, k = 13; j < number_of_banks;
                                     ++j, ++k) {
                                    payment_methods.push_back(
                                        market_payment_methods[n % k]
                                    );
                                }
                                min_available = 6;
                                available =
                                    min_available + n % 5994 +
                                    dec::decimal_cast<8>(std::to_string(
                                        ((double)(n % 1000001) / 1000000)
                                    ));
                                ;
                                upper_limit =
                                    dec::decimal_cast<8>(price) * available;
                                if (available > 90) {
                                    upper_limit *=
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 71) / 100)
                                        )) +
                                        "0.3";
                                }
                                lower_limit =
                                    500 + (n % std::stoi(decimal2frp_to_string(
                                                   upper_limit - 500
                                               )));
                                lower_limit += std::to_string(
                                    (double)((n + 313) % 101) / 100
                                );
                                break;

                            case Directions::sell:
                                average_price = 93.75;
                                price =
                                    average_price +
                                    std::to_string(
                                        (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                        ((double)(n % 76) / 100)
                                    );
                                number_of_banks = n % 6;
                                if (number_of_banks == 0) {
                                    number_of_banks = 1;
                                }
                                for (int j = 0, k = 13; j < number_of_banks;
                                     ++j, ++k) {
                                    payment_methods.push_back(
                                        market_payment_methods[n % k]
                                    );
                                }
                                min_available = 6;
                                available =
                                    min_available + n % 5994 +
                                    dec::decimal_cast<8>(std::to_string(
                                        ((double)(n % 1000001) / 1000000)
                                    ));
                                ;
                                upper_limit =
                                    dec::decimal_cast<8>(price) * available;
                                if (available > 90) {
                                    upper_limit *=
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 71) / 100)
                                        )) +
                                        "0.3";
                                }
                                lower_limit =
                                    500 + (n % std::stoi(decimal2frp_to_string(
                                                   upper_limit - 500
                                               )));
                                lower_limit += std::to_string(
                                    (double)((n + 313) % 101) / 100
                                );
                                break;
                        }
                        break;

                    case Coins::USDT:
                        switch (direction) {
                            case Directions::buy:
                                average_price = 94.85;
                                price =
                                    average_price +
                                    std::to_string(
                                        (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                        ((double)(n % 26) / 100)
                                    );
                                number_of_banks = n % 6;
                                if (number_of_banks == 0) {
                                    number_of_banks = 1;
                                }
                                for (int j = 0, k = 13; j < number_of_banks;
                                     ++j, ++k) {
                                    payment_methods.push_back(
                                        market_payment_methods[n % k]
                                    );
                                }
                                min_available = 6;
                                available = min_available + n % 49994 +
                                            dec::decimal_cast<8>(std::to_string(
                                                ((double)(n % 10001) / 10000)
                                            ));
                                upper_limit =
                                    dec::decimal_cast<8>(price) * available;
                                if (available > 90) {
                                    upper_limit *=
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 71) / 100)
                                        )) +
                                        "0.3";
                                }
                                lower_limit =
                                    500 + (n % std::stoi(decimal2frp_to_string(
                                                   upper_limit - 500
                                               )));
                                lower_limit += std::to_string(
                                    (double)((n + 313) % 101) / 100
                                );
                                break;

                            case Directions::sell:
                                average_price = 93.50;
                                price =
                                    average_price +
                                    std::to_string(
                                        (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                        ((double)(n % 301) / 100)
                                    );
                                number_of_banks = n % 6;
                                if (number_of_banks == 0) {
                                    number_of_banks = 1;
                                }
                                for (int j = 0, k = 13; j < number_of_banks;
                                     ++j, ++k) {
                                    payment_methods.push_back(
                                        market_payment_methods[n % k]
                                    );
                                }
                                min_available = 6;
                                available = min_available + n % 49994 +
                                            dec::decimal_cast<8>(std::to_string(
                                                ((double)(n % 10001) / 10000)
                                            ));
                                upper_limit =
                                    dec::decimal_cast<8>(price) * available;
                                if (available > 90) {
                                    upper_limit *=
                                        dec::decimal_cast<2>(std::to_string(
                                            ((double)(n % 71) / 100)
                                        )) +
                                        "0.3";
                                }
                                lower_limit =
                                    500 + (n % std::stoi(decimal2frp_to_string(
                                                   upper_limit - 500
                                               )));
                                lower_limit += std::to_string(
                                    (double)((n + 313) % 101) / 100
                                );
                                break;
                        }
                        break;
                }
                break;
        }
        orders.push_back(std::move(std::make_unique<order>(
            currency, coin, direction, price, payment_methods, lower_limit,
            upper_limit, available, link
        )));
    }
}

void bybit_simulator::update_spot_rates() {
    int n, isOdd;
    std::pair<Coins, Coins> spot_pair;
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            n = gen() % 32767;
            isOdd = n & 1;
            spot_pair.first = coins[i];
            spot_pair.second = coins[j];
            switch (spot_pair.first) {
                case Coins::USDT:
                    switch (spot_pair.second) {
                        case Coins::USDC: {
                            dec::decimal<8> rate("1.0016");
                            spot_rates[spot_pair.first][spot_pair.second] =
                                rate +
                                dec::decimal_cast<8>(isOdd * (-1)) *
                                    (dec::decimal_cast<8>(n % 16) *
                                     dec::decimal_cast<8>("0.0001")) +
                                dec::decimal_cast<8>((int)!isOdd) *
                                    (dec::decimal_cast<8>(n % 2) *
                                     dec::decimal_cast<8>("0.0001"));
                        } break;

                        case Coins::BTC: {
                            dec::decimal<8> rate("57134.38");
                            spot_rates[spot_pair.first][spot_pair.second] =
                                rate +
                                dec::decimal_cast<8>(isOdd * (-1)) *
                                    (dec::decimal_cast<8>(n % 59207) *
                                     dec::decimal_cast<8>("0.01")) +
                                dec::decimal_cast<8>((int)!isOdd) *
                                    (dec::decimal_cast<8>(n % 443336) *
                                     dec::decimal_cast<8>("0.01"));
                        } break;

                        case Coins::ETH: {
                            dec::decimal<8> rate("2886.20");
                            spot_rates[spot_pair.first][spot_pair.second] =
                                rate +
                                dec::decimal_cast<8>(isOdd * (-1)) *
                                    (dec::decimal_cast<8>(n % 6969) *
                                     dec::decimal_cast<8>("0.01")) +
                                dec::decimal_cast<8>((int)!isOdd) *
                                    (dec::decimal_cast<8>(n % 15156) *
                                     dec::decimal_cast<8>("0.01"));
                        } break;
                    }
                    break;

                case Coins::USDC:
                    switch (spot_pair.second) {
                        case Coins::BTC: {
                            dec::decimal<8> rate("57254.59");
                            spot_rates[spot_pair.first][spot_pair.second] =
                                rate +
                                dec::decimal_cast<8>(isOdd * (-1)) *
                                    (dec::decimal_cast<8>(n % 77084) *
                                     dec::decimal_cast<8>("0.01")) +
                                dec::decimal_cast<8>((int)!isOdd) *
                                    (dec::decimal_cast<8>(n % 427861) *
                                     dec::decimal_cast<8>("0.01"));
                        } break;

                        case Coins::ETH: {
                            dec::decimal<8> rate("2879.50");
                            spot_rates[spot_pair.first][spot_pair.second] =
                                rate +
                                dec::decimal_cast<8>(isOdd * (-1)) *
                                    (dec::decimal_cast<8>(n % 6619) *
                                     dec::decimal_cast<8>("0.01")) +
                                dec::decimal_cast<8>((int)!isOdd) *
                                    (dec::decimal_cast<8>(n % 15616) *
                                     dec::decimal_cast<8>("0.01"));
                        } break;
                    }
                    break;

                case Coins::BTC: {
                    dec::decimal<8> rate("0.050279");
                    spot_rates[spot_pair.first][spot_pair.second] =
                        rate +
                        dec::decimal_cast<8>(isOdd * (-1)) *
                            (dec::decimal_cast<8>(n % 1433) *
                             dec::decimal_cast<8>("0.000001")) +
                        dec::decimal_cast<8>((int)!isOdd) *
                            (dec::decimal_cast<8>(n % 57) *
                             dec::decimal_cast<8>("0.000001"));
                } break;
            }
        }
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

void bybit_simulator::pack_in_json() {
    json_orders.clear();
    nlohmann::json json_order{};
    for (auto &i : orders) {
        json_order["market"] = magic_enum::enum_name(market_);
        json_order["currency"] = magic_enum::enum_name(i->currency);
        json_order["coin"] = magic_enum::enum_name(i->coin);
        json_order["direction"] = magic_enum::enum_name(i->direction);
        json_order["price"] = decimal2_to_string(i->price);
        json_order["payment_methods"] = local_bank_is(i->payment_methods);
        json_order["lower_limit"] = decimal2_to_string(i->lower_limit);
        json_order["upper_limit"] = decimal2frp_to_string(i->upper_limit);
        json_order["available"] = decimal8_to_string(i->available);
        json_order["link"] = i->link;
        json_orders.push_back(json_order);
        json_order.erase(json_order.begin(), json_order.end());
    }
    json_spot_rates["market"] = magic_enum::enum_name(market_);
    for (int j = 0; j < 4; ++j) {
        for (int k = j + 1; k < 4; ++k) {
            switch (coins[j]) {
                case Coins::USDT:
                    switch (coins[k]) {
                        case Coins::USDC:
                            json_spot_rates["USDC/USDT"] = decimal8_to_string(
                                spot_rates[coins[j]][coins[k]]
                            );
                            break;

                        case Coins::BTC:
                            json_spot_rates["BTC/USDT"] = decimal8_to_string(
                                spot_rates[coins[j]][coins[k]]
                            );
                            break;

                        case Coins::ETH:
                            json_spot_rates["ETH/USDT"] = decimal8_to_string(
                                spot_rates[coins[j]][coins[k]]
                            );
                            break;
                    }
                    break;

                case Coins::USDC:
                    switch (coins[k]) {
                        case Coins::BTC:
                            json_spot_rates["BTC/USDC"] = decimal8_to_string(
                                spot_rates[coins[j]][coins[k]]
                            );
                            break;

                        case Coins::ETH:
                            json_spot_rates["ETH/USDC"] = decimal8_to_string(
                                spot_rates[coins[j]][coins[k]]
                            );
                            break;
                    }
                    break;

                case Coins::BTC:
                    json_spot_rates["ETH/BTC"] =
                        decimal8_to_string(spot_rates[coins[j]][coins[k]]);
                    break;
            }
        }
    }
}

void bybit_simulator::update_market() {
    this->update_market_orders();
    this->update_spot_rates();
    this->pack_in_json();
}

bybit::bybit() {
    market_ = Markets::bybit;
}

using boost::asio::ip::tcp;

void bybit::update_market() {
    std::string orders = "";
    boost::asio::io_context io_context;
    auto create_connection = [&]() {
        tcp::socket s(io_context);
        boost::asio::connect(
            s, tcp::resolver(io_context).resolve("127.0.0.1", "19379")
        );
        return tcp::iostream(std::move(s));
    };
    tcp::iostream conn = create_connection();
    conn.socket().shutdown(tcp::socket::shutdown_send);
    int c;
    std::string symbol;
    while ((c = conn.get()) != std::char_traits<char>::eof()) {
        symbol = static_cast<char>(c);
        orders += symbol;
    }
    std::ofstream temp_json("/Users/exchange scraper_/jsons/temp.json");
    temp_json << orders;
    temp_json.close();
    std::ifstream temp_json_("/Users/exchange scraper_/jsons/temp.json");
    nlohmann::json result;
    temp_json_ >> result;
    json_orders = result["orders"];
    json_spot_rates = result["spot_rates"];
    temp_json_.close();
}

htx::htx() {
    market_ = Markets::htx;
}

void htx::update_market() {
    std::string orders = "";
    boost::asio::io_context io_context;
    auto create_connection = [&]() {
        tcp::socket s(io_context);
        boost::asio::connect(
            s, tcp::resolver(io_context).resolve("127.0.0.1", "19379")
        );
        return tcp::iostream(std::move(s));
    };
    tcp::iostream conn = create_connection();
    conn.socket().shutdown(tcp::socket::shutdown_send);
    int c;
    std::string symbol;
    while ((c = conn.get()) != std::char_traits<char>::eof()) {
        symbol = static_cast<char>(c);
        orders += symbol;
    }
    std::ofstream temp_json("/Users/exchange scraper_/jsons/temp.json");
    temp_json << orders;
    temp_json.close();
    std::ifstream temp_json_("/Users/exchange scraper_/jsons/temp.json");
    nlohmann::json result;
    temp_json_ >> result;
    json_orders = result["orders"];
    json_spot_rates = result["spot_rates"];
    temp_json_.close();
}