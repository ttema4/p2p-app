#ifndef EXCHANGES_HPP_
#define EXCHANGES_HPP_

#include "decimal/decimal.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <random>

enum class Exchanges { bybit_simulator };

enum class Currencies { RUB };

enum class Coins { BTC, ETH, USDC, USDT };

enum class Directions { buy, sell };

struct order {
  Exchanges exchange;
  Currencies currency;
  Coins coin;
  Directions direction;
  dec::decimal<2> price;
  std::vector<std::string> payment_methods;
  dec::decimal<2> lower_limit;
  dec::decimal<2, dec::floor_round_policy> upper_limit;
  dec::decimal<8> available;
  std::string link;

  order(Exchanges exchange_, Currencies currency_, Coins coin_,
        Directions direction_, dec::decimal<2> price_,
        std::vector<std::string> &payment_methods_,
        dec::decimal<2> lower_limit_,
        dec::decimal<2, dec::floor_round_policy> upper_limit_,
        dec::decimal<8> available_, std::string &link_);
};

struct exchange {
  std::vector<std::string> exchange_payment_methods;
  std::vector<std::unique_ptr<order>> orders;

    virtual ~exchange() = default;

  virtual void update_exchange_orders() = 0;
};

struct bybit_simulator : exchange {
  std::mt19937 gen;

  bybit_simulator();

  void update_exchange_orders() override;
};

std::string decimal2_to_string(dec::decimal<2> n);

std::string decimal2frp_to_string(dec::decimal<2, dec::floor_round_policy> n);

std::string decimal8_to_string(dec::decimal<8> n);

#endif