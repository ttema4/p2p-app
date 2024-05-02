#include <chrono>
#include <iomanip>
#include <thread>
#include "../scraper.hpp"

int main() {
    auto fake_market = std::make_unique<bybit_simulator>();
    scraper test_scraper;
    test_scraper.add_market(std::move(fake_market));
    while (true) {
        test_scraper.update_markets();
        test_scraper.handle();
        std::ofstream out;
        out.open("test_result.txt");
        out << "MARKET | CURRENCY | COIN | DIRECTION | PRICE | [ PAYMENT "
               "METHODS ] "
               "| LOWER "
               "LIMIT | UPPER LIMIT | AVAILABLE | LINK\n\n";
        for (auto &i : test_scraper.markets) {
            for (auto &j : i->orders) {
                if (out.is_open()) {
                    out << magic_enum::enum_name(i->market_) << " | "
                        << magic_enum::enum_name(j->currency) << " | "
                        << magic_enum::enum_name(j->coin) << " | "
                        << magic_enum::enum_name(j->direction) << " | "
                        << j->price << " | [ ";
                    for (auto &k : local_bank_is(j->payment_methods)) {
                        out << k << " ";
                    }
                    out << "] | " << j->lower_limit << " | " << j->upper_limit
                        << " | " << j->available << " | " << j->link << "\n\n";
                }
            }
        }
        out << "MARKET | SPOT PAIR | RATE\n\n";
        for (auto &i : test_scraper.markets) {
            for (int j = 0; j < 4; ++j) {
                for (int k = j + 1; k < 4; ++k) {
                    out << magic_enum::enum_name(i->market_) << " | "
                        << magic_enum::enum_name(coins[k]) << "/"
                        << magic_enum::enum_name(coins[j]) << " | "
                        << i->spot_rates[coins[j]][coins[k]] << "\n\n";
                }
            }
        }
        out.close();
        std::cout << "updated\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
}