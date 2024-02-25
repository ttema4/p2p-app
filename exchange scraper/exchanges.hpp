#include <iostream>
#include <memory>
#include <string>
#include <vector>

enum class Currencies { RUB };

enum class Coins { BTC, ETH, USDC, USDT };

enum class Directions { buy, sell };

struct order {
  Currencies currency;
  Coins coin;
  Directions direction;
  double price;
  std::vector<std::string> payment_methods;
  double lower_limit;
  double upper_limit;
  double available;
  std::string link;

  order(Currencies currency_, Coins coin_, Directions direction_, double price_,
        std::vector<std::string> &payment_methods_, double lower_limit_,
        double upper_limit_, double available_, std::string &link_);
};

enum class Exchanges { bybit_simulator_ };

struct exchange {
  Exchanges name;
  std::vector<std::string> exchange_payment_methods;
  std::vector<std::unique_ptr<order>> orders;

  virtual ~exchange() = default;

  virtual void update_orders() = 0;
};

struct bybit_simulator : exchange {
  bybit_simulator();

  void update_orders() override;
};