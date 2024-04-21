#ifndef LOGIC_SIMULATOR_
#define LOGIC_SIMULATOR_

#include "logic_fwd.hpp"
#include "src/nlohmann/json.hpp"

#include <string>
#include <vector>
#include <random>
#include <vector>


inline std::vector<std::string> types {"BUY", "SELL"};
inline std::vector<std::string> ids {"https://docs.google.com/spreadsheets/u/0/d/1W2VigJfqsPFK12JuIj62l1kIaeJOE03xK-PTkzmMh2E/htmlview", "https://youtu.be/QMMgjjGugHE?si=PVyBYphuinGCxVun", "https://www.youtube.com/watch?v=dQw4w9WgXcQ", "https://youtu.be/MGkWSAnoDOI?si=IyLCYPO6QFl2-b-Y&t=330"};
inline std::vector<std::string> coins {"BTC", "ETH", "DOGY", "SOLU", "WLD", "COTI"};
inline std::vector<std::string> banks {"Tinkoff", "Sber", "Alpha", "Qiwi", "SBP"};


inline std::mt19937 gen;

inline Chain getFakeChain() {
    std::string fake_type1 = types[gen() % types.size()];
    std::string fake_id1 = ids[gen() % ids.size()];
    double fake_rating1 = (double)(gen() % 500) / 100;
    std::string fake_coin11 = "USDT";
    std::string fake_coin21 = coins[gen() % coins.size()];
    std::string fake_bank1 = banks[gen() % banks.size()];
    std::pair<int, int> fake_min_max1 = {gen() % 100, gen() % 1000000 + 100};
    double fake_exchange_rate1 = (double)(gen() % 1000000) / 100;

    Order order1(fake_type1, fake_id1, fake_rating1, fake_coin11, fake_coin21, fake_bank1, fake_min_max1, fake_exchange_rate1);

    std::string fake_type2 = types[gen() % types.size()];
    std::string fake_id2 = ids[gen() % ids.size()];
    double fake_rating2 = (double)(gen() % 500) / 100;
    std::string fake_coin12 = coins[gen() % coins.size()];
    std::string fake_coin22 = "USDT";
    std::string fake_bank2 = banks[gen() % banks.size()];
    std::pair<int, int> fake_min_max2 = {gen() % 100, gen() % 1000000 + 100};
    double fake_exchange_rate2 = (double)(gen() % 1000000) / 100;

    Order order2(fake_type2, fake_id2, fake_rating2, fake_coin12, fake_coin22, fake_bank2, fake_min_max2, fake_exchange_rate2);

    std::pair<std::string, std::string> fake_change {fake_coin21, fake_coin12};

    double fake_spread = (double)(gen() % 100) / 10;

    Chain fake_chain(order1, fake_change, order2, fake_spread);

    return fake_chain;
}

inline std::string getJsonToString() {
    int n = gen() % 91 + 10;
    std::vector<Chain> chains;
    for (int i = 0; i < n; i++) chains.push_back(getFakeChain());
    nlohmann::json newjson = chains;
    return newjson.dump();
}

#endif // LOGIC_SIMULATOR_

// int main() {
//     for (int i = 0; i < 10; i++) {
//         std::cout << getJsonToString() << std::endl;
//     }
// }


// int main() {
//     for (int i = 0; i < 10; i++) {
//         Chain new_chain = getFakeChain();

//         std::cout << std::endl;

//         std::cout << "Fake type: " << new_chain.buy.type << std::endl;
//         std::cout << "Fake id: " << new_chain.buy.id << std::endl;
//         std::cout << "Fake reting: " << new_chain.buy.seller_rating << std::endl;
//         std::cout << "Fake coin1: " << new_chain.buy.coin1 << std::endl;
//         std::cout << "Fake coin2: " << new_chain.buy.coin2 << std::endl;
//         std::cout << "Fake bank: " << new_chain.buy.bank << std::endl;
//         std::cout << "Fake min max: " << new_chain.buy.min_max.first << " " << new_chain.buy.min_max.second << std::endl;
//         std::cout << "Fake exchange rate: " << new_chain.buy.exchange_rate << std::endl;

//         std::cout << std::endl;
//         std::cout << "Fake change: " << new_chain.change.first << " -> " << new_chain.change.second << std::endl;
//         std::cout << std::endl;

//         std::cout << "Fake type: " << new_chain.sell.type << std::endl;
//         std::cout << "Fake id: " << new_chain.sell.id << std::endl;
//         std::cout << "Fake reting: " << new_chain.sell.seller_rating << std::endl;
//         std::cout << "Fake coin1: " << new_chain.sell.coin1 << std::endl;
//         std::cout << "Fake coin2: " << new_chain.sell.coin2 << std::endl;
//         std::cout << "Fake bank: " << new_chain.sell.bank << std::endl;
//         std::cout << "Fake min max: " << new_chain.sell.min_max.first << " " << new_chain.buy.min_max.second << std::endl;
//         std::cout << "Fake exchange rate: " << new_chain.sell.exchange_rate << std::endl;

//         std::cout << std::endl << "----------------------" << std::endl;
//     }
// }
