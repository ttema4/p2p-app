#include "data_processing.hpp"

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "storage_structures.hpp"

namespace p2p {

Chains Analysis::analyze(Orders &orders,
                       MarketRates &market_rates) {

    Chains chains;
    for(const std::string& current_market : markets) {
        Orders orders_for_sell;
        Orders orders_for_buy;
        for (Order order : orders.list) {
            if (order.type == "sell" && order.market == current_market) {
                orders_for_sell.list.push_back(order);
            } else if (order.type == "buy" && order.market == current_market) {
                orders_for_buy.list.push_back(order);
            }
        }

        if (ORDERS_FOR_BUY_LOGS_ON) {
            std::cout << "Market: " << current_market << std::endl;
            std::cout << "orders_for_buy size: " << orders_for_buy.list.size()
                    << std::endl;
            for (int i = 0; i < 5; i++) {
                std::cout << i << ". We buy: " << orders_for_buy.list[i].coin2
                        << " for " << orders_for_buy.list[i].coin1 << " by price "
                        << orders_for_buy.list[i].exchange_rate << std::endl;
            }
        }

        if (ORDERS_FOR_SELL_LOGS_ON) {
            std::cout << "Market: " << current_market << std::endl;
            std::cout << "orders_for_sell size: " << orders_for_sell.list.size()
                    << std::endl;
            for (int i = 0; i < orders_for_sell.list.size(); i++) {
                std::cout << i << ". We sell: " << orders_for_sell.list[i].coin2
                        << " for " << orders_for_sell.list[i].coin1 << " by price "
                        << orders_for_sell.list[i].exchange_rate << std::endl;
            }
        }

        std::unordered_map<std::string, std::vector<Order>> sell_by_coin;
        for (Order sell : orders_for_sell.list) {
            sell_by_coin[sell.coin2].push_back(sell);
        }

        if (SELL_BY_COIN_LOGS_ON) {
            std::cout << "Market: " << current_market << std::endl;
            std::cout << "sell_by_coin size: " << sell_by_coin.size() << std::endl;
            std::cout << "sell_by_coin:" << std::endl;
            for (auto it = sell_by_coin.begin(); it != sell_by_coin.end(); ++it) {
                std::cout << it->first << std::endl;
            }
        }

        if (MARKET_RATES_LOGS_ON) {
            std::cout << "Market: " << current_market << std::endl;
            std::cout << "market_rates:" << std::endl;
            for (auto it = market_rates.list[current_market].begin(); it != market_rates.list[current_market].end();
                ++it) {
                for (auto it2 = it->second.begin(); it2 != it->second.end();
                    ++it2) {
                    std::cout << it->first << " " << it2->first << " "
                            << it2->second << std::endl;
                }
            }
        }

        std::unordered_map<std::string, Chain> chains_map;

        for (Order buy : orders_for_buy.list) {
            long double current_sum_1 = 1000;
            std::string coin_buy = buy.coin2;
            current_sum_1 = current_sum_1 / buy.exchange_rate;

            for (std::pair<std::string, long double> market : market_rates.list[current_market][coin_buy]) {
                long double current_sum_2 = current_sum_1;
                std::string coin_sell = market.first;
                long double rate = market.second;
                current_sum_2 = current_sum_2 * rate;

                for (Order sell : sell_by_coin[coin_sell]) {
                    long double current_sum_3 = current_sum_2;
                    current_sum_3 = current_sum_3 * sell.exchange_rate;
                    long double spread = (current_sum_3 / 1000 - 1) * 100;
                    spread = std::round(spread * 1000) / 1000;
                    Chain chain = Chain(buy, std::make_pair(coin_buy, coin_sell),
                                        sell, spread);
                    std::string key = coin_buy + "-" + coin_sell;

                    if (chains_map.find(key) == chains_map.end()) {
                        chains_map[key] = chain;
                    } else {
                        if (chains_map[key].spread < spread) {
                            chains_map[key] = chain;
                        }
                    }
                }
            }
        }

        for (auto it = chains_map.begin(); it != chains_map.end(); ++it) {
            chains.list.push_back(it->second);
        }
    }

    return chains;
}

}  // namespace p2p
