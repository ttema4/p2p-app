#include "exchanges.hpp"
#include <cmath>
#include <random>

order::order(Currencies currency_, Coins coin_, Directions direction_,
             double price_, std::vector<std::string> &payment_methods_,
             double lower_limit_, double upper_limit_, double available_,
             std::string &link_)
    : currency(currency_), coin(coin_), direction(direction_), price(price_),
      payment_methods(payment_methods_), lower_limit(lower_limit_),
      upper_limit(upper_limit_), available(available_), link(link_) {}

bybit_simulator::bybit_simulator() {
  name = Exchanges::bybit_simulator_;
  exchange_payment_methods = {"Local Card(Yellow)",
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

void bybit_simulator::update_orders() {
  Currencies currencies_[]{Currencies::RUB};
  Coins coins_[]{Coins::BTC, Coins::ETH, Coins::USDC, Coins::USDT};
  Directions directions_[]{Directions::buy, Directions::sell};
  std::mt19937 gen;
  double price;
  double average_price;
  std::vector<std::string> payment_methods;
  int number_of_banks;
  double lower_limit;
  double upper_limit;
  double available;
  double min_available;
  std::string link =
      "https://music.youtube.com/channel/UCP2Q30XUqNQXstaRolRQ_hw";
  for (int i = 0; i < 1052; ++i) {
    int n = gen() % 32767;
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
                  (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) * (n % 70001) +
                  ((double)(n % 101) / 100);
          number_of_banks = n % 6;
          if (number_of_banks == 0) {
            number_of_banks = 1;
          }
          for (int j = 0, k = 13; j < number_of_banks; ++j, ++k) {
            payment_methods.push_back(exchange_payment_methods[n % k]);
          }
          min_available = 0.00013;
          available = min_available + ((double)(n % 49988) / 100000) +
                      ((double)(n % 1000000001) / 1000000000);
          upper_limit = std::floor(price * available);
          if (available > 0.00195) {
            upper_limit *= 0.3 + ((double)(n % 71) / 100);
          }
          upper_limit -= (double)(n % 101) / 100;
          lower_limit = 500 + (n % (int)(upper_limit - 500));
          lower_limit += (double)((n + 313) % 101) / 100;
          break;

        case Directions::sell:
          average_price = 4840000;
          price = average_price +
                  (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) * (n % 30001) +
                  ((double)(n % 101) / 100);
          number_of_banks = n % 6;
          if (number_of_banks == 0) {
            number_of_banks = 1;
          }
          for (int j = 0, k = 13; j < number_of_banks; ++j, ++k) {
            payment_methods.push_back(exchange_payment_methods[n % k]);
          }
          min_available = 0.00013;
          available = min_available + ((double)(n % 49988) / 100000) +
                      ((double)(n % 1000000001) / 1000000000);
          upper_limit = std::floor(price * available);
          if (available > 0.00195) {
            upper_limit *= 0.3 + ((double)(n % 71) / 100);
          }
          upper_limit -= (double)(n % 101) / 100;
          lower_limit = 500 + (n % (int)(upper_limit - 500));
          lower_limit += (double)((n + 313) % 101) / 100;
          break;
        }
        break;

      case Coins::ETH:
        switch (direction) {
        case Directions::buy:
          average_price = 294000;
          price = average_price +
                  (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) * (n % 6001) +
                  ((double)(n % 101) / 100);
          number_of_banks = n % 6;
          if (number_of_banks == 0) {
            number_of_banks = 1;
          }
          for (int j = 0, k = 13; j < number_of_banks; ++j, ++k) {
            payment_methods.push_back(exchange_payment_methods[n % k]);
          }
          min_available = 0.00200000;
          available =
              min_available + n % 11 + ((double)(n % 1000000001) / 1000000000);
          upper_limit = std::floor(price * available);
          if (available > 0.3) {
            upper_limit *= 0.3 + ((double)(n % 71) / 100);
          }
          upper_limit -= (double)(n % 101) / 100;
          lower_limit = 500 + (n % (int)(upper_limit - 500));
          lower_limit += (double)((n + 313) % 101) / 100;
          break;

        case Directions::sell:
          average_price = 283000;
          price = average_price +
                  (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) * (n % 3001) +
                  ((double)(n % 101) / 100);
          number_of_banks = n % 6;
          if (number_of_banks == 0) {
            number_of_banks = 1;
          }
          for (int j = 0, k = 13; j < number_of_banks; ++j, ++k) {
            payment_methods.push_back(exchange_payment_methods[n % k]);
          }
          min_available = 0.00200000;
          available =
              min_available + n % 11 + ((double)(n % 1000000001) / 1000000000);
          upper_limit = std::floor(price * available);
          if (available > 0.3) {
            upper_limit *= 0.3 + ((double)(n % 71) / 100);
          }
          upper_limit -= (double)(n % 101) / 100;
          lower_limit = 500 + (n % (int)(upper_limit - 500));
          lower_limit += (double)((n + 313) % 101) / 100;
          break;
        }
        break;

      case Coins::USDC:
        switch (direction) {
        case Directions::buy:
          average_price = 98.50;
          price = average_price + (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                      ((double)(n % 301) / 100);
          number_of_banks = n % 6;
          if (number_of_banks == 0) {
            number_of_banks = 1;
          }
          for (int j = 0, k = 13; j < number_of_banks; ++j, ++k) {
            payment_methods.push_back(exchange_payment_methods[n % k]);
          }
          min_available = 6;
          available =
              min_available + n % 5994 + ((double)(n % 1000001) / 1000000);
          ;
          upper_limit = std::floor(price * available);
          if (available > 90) {
            upper_limit *= 0.3 + ((double)(n % 71) / 100);
          }
          upper_limit -= (double)(n % 101) / 100;
          lower_limit = 500 + (n % (int)(upper_limit - 500));
          lower_limit += (double)((n + 313) % 101) / 100;
          break;

        case Directions::sell:
          average_price = 93.75;
          price = average_price + (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                      ((double)(n % 76) / 100);
          number_of_banks = n % 6;
          if (number_of_banks == 0) {
            number_of_banks = 1;
          }
          for (int j = 0, k = 13; j < number_of_banks; ++j, ++k) {
            payment_methods.push_back(exchange_payment_methods[n % k]);
          }
          min_available = 6;
          available =
              min_available + n % 5994 + ((double)(n % 1000001) / 1000000);
          ;
          upper_limit = std::floor(price * available);
          if (available > 90) {
            upper_limit *= 0.3 + ((double)(n % 71) / 100);
          }
          upper_limit -= (double)(n % 101) / 100;
          lower_limit = 500 + (n % (int)(upper_limit - 500));
          lower_limit += (double)((n + 313) % 101) / 100;
          break;
        }
        break;

      case Coins::USDT:
        switch (direction) {
        case Directions::buy:
          average_price = 94.85;
          price = average_price + (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                      ((double)(n % 26) / 100);
          number_of_banks = n % 6;
          if (number_of_banks == 0) {
            number_of_banks = 1;
          }
          for (int j = 0, k = 13; j < number_of_banks; ++j, ++k) {
            payment_methods.push_back(exchange_payment_methods[n % k]);
          }
          min_available = 6;
          available = min_available + n % 49994 + ((double)(n % 10001) / 10000);
          upper_limit = std::floor(price * available);
          if (available > 90) {
            upper_limit *= 0.3 + ((double)(n % 71) / 100);
          }
          upper_limit -= (double)(n % 101) / 100;
          lower_limit = 500 + (n % (int)(upper_limit - 500));
          lower_limit += (double)((n + 313) % 101) / 100;
          break;

        case Directions::sell:
          average_price = 93.50;
          price = average_price + (-1 * (n % 2 == 0) + 1 * (n % 2 != 0)) *
                                      ((double)(n % 301) / 100);
          number_of_banks = n % 6;
          if (number_of_banks == 0) {
            number_of_banks = 1;
          }
          for (int j = 0, k = 13; j < number_of_banks; ++j, ++k) {
            payment_methods.push_back(exchange_payment_methods[n % k]);
          }
          min_available = 6;
          available = min_available + n % 49994 + ((double)(n % 10001) / 10000);
          upper_limit = std::floor(price * available);
          if (available > 90) {
            upper_limit *= 0.3 + ((double)(n % 71) / 100);
          }
          upper_limit -= (double)(n % 101) / 100;
          lower_limit = 500 + (n % (int)(upper_limit - 500));
          lower_limit += (double)((n + 313) % 101) / 100;
          break;
        }
        break;
      }
      break;
    }
    orders.push_back(std::move(std::make_unique<order>(
        currency, coin, direction, price, payment_methods, lower_limit,
        upper_limit, available, link)));
  }
}