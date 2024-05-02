#ifndef MARKETS_HPP_
#define MARKETS_HPP_

#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include "decimal/decimal.h"

enum class Markets { bybit_simulator };

enum class Currencies { RUB };

enum class Coins { BTC, ETH, USDC, USDT };

inline const Coins coins[] = {Coins::USDT, Coins::USDC, Coins::BTC, Coins::ETH};

enum class Directions { buy, sell };

struct order {
    Currencies currency;
    Coins coin;
    Directions direction;
    dec::decimal<2> price;
    std::vector<std::string> payment_methods;
    dec::decimal<2> lower_limit;
    dec::decimal<2, dec::floor_round_policy> upper_limit;
    dec::decimal<8> available;
    std::string link;

    order(
        Currencies currency_,
        Coins coin_,
        Directions direction_,
        dec::decimal<2> price_,
        std::vector<std::string> &payment_methods_,
        dec::decimal<2> lower_limit_,
        dec::decimal<2, dec::floor_round_policy> upper_limit_,
        dec::decimal<8> available_,
        std::string &link_
    );
};

struct market {
    Markets market_;
    std::vector<std::string> market_payment_methods;
    std::vector<std::unique_ptr<order>> orders;
    std::unordered_map<Coins, std::unordered_map<Coins, dec::decimal<8>>>
        spot_rates;

    virtual ~market() = default;

    virtual void update_market_orders() = 0;

    virtual void update_spot_rates() = 0;

    virtual void update_market() = 0;
};

struct bybit_simulator : market {
    std::mt19937 gen;

    bybit_simulator();

    void update_market_orders() override;

    void update_spot_rates() override;

    void update_market() override;
};

std::string decimal2_to_string(dec::decimal<2> n);

std::string decimal2frp_to_string(dec::decimal<2, dec::floor_round_policy> n);

std::string decimal8_to_string(dec::decimal<8> n);

#endif