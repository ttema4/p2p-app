#include <chrono>
#include <iomanip>
#include <thread>
#include "../scraper.hpp"

int main() {
    auto fake_market = std::make_unique<bybit_simulator>();
    scraper test_scraper;
    test_scraper.add_market(std::move(fake_market));
    while (true) {
        test_scraper.update_markets_orders();
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
                    out << magic_enum::enum_name(j->market) << " | "
                        << magic_enum::enum_name(j->currency) << " | "
                        << magic_enum::enum_name(j->coin) << " | "
                        << magic_enum::enum_name(j->direction) << " | "
                        << j->price << " | [ ";
                    for (auto &k : j->payment_methods) {
                        out << k << " ";
                    }
                    out << "] | " << j->lower_limit << " | " << j->upper_limit
                        << " | " << j->available << " | " << j->link << "\n\n";
                }
            }
        }
        out.close();
        std::cout << "updated\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }
}