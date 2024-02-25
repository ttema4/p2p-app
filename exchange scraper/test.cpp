#include "magic_enum.hpp"
#include "scraper.hpp"
#include <fstream>
#include <iomanip>

int main() {
  auto fake_exchange = new bybit_simulator();
  scraper test_scraper;
  test_scraper.add_exchange(std::move(fake_exchange));
  test_scraper.exchanges[0]->update_orders();
  std::ofstream out;
  out.open("test_result.txt");
  out << "CURRENCY | COIN | DIRECTION | PRICE | [ PAYMENT METHODS ] | LOWER "
         "LIMIT | UPPER LIMIT | AVAILABLE | LINK\n\n";
  for (auto &i : test_scraper.exchanges) {
    for (auto &j : i->orders) {
      if (out.is_open()) {
        out << magic_enum::enum_name(j->currency) << " | "
            << magic_enum::enum_name(j->coin) << " | "
            << magic_enum::enum_name(j->direction) << " | " << j->price
            << " | [ ";
        for (auto &k : j->payment_methods) {
          out << k << " ";
        }
        out << "] | " << j->lower_limit << " | " << j->upper_limit << " | "
            << j->available << " | " << j->link << "\n\n";
      }
    }
  }
  out.close();
}