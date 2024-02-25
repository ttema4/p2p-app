#include <string>
#include <vector>
#include <random>
#include <iostream>


// #include <nlohmann/json.hpp>

// int main() {
//     nlohmann::json j;
//     j["name"] = "John";
//     j["age"] = 30;
//     j["is_student"] = false;
//     j["subjects"] = {"Math", "Physics"};
// }


struct Order {
    // Фактически, без статуса мерчанта на бирже(получить его очень сложно,
    // а потому в MVP уж точно не стоит рассматривать свзки с учетом того,
    // что закупаемся/продаемся как мерчант) мы всегда в роли покупателя,
    // только при закупке "покупаем монету за рубли", а при продаже "покупаем
    // рубли за монету". Несмотря на это, чтобы не смешивать все ордера, мы будем
    // условно помечать тип ордера как "BUY" или "SELL".
    std::string type;

    std::string id; // Строка, храним не фактический id, а фрагмент ссылки(?)
    long double seller_rating; // Рейтинг продавца

    // Валюта, ЗА которую мы ПОКУПАЕМ. На первом шаге связки - условность, 
    // так как в MVP(да и вообще имея на руках только карты российских банков)
    // логично реализовать только свзки с покупкой за рубли.
    std::string coin1;
    std::string coin2; // Валюта, ЗА которую мы ПРОДАЁМ. Аналогично, на последнем шаге всегда рубль.
    std::string bank; // Банк
    std::pair<int, int> min_max; // Минимальная и максимальная сумма сделки
    long double exchange_rate; // Курс

    Order(std::string type_, std::string id_, long double seller_rating_, std::string coin1_,
            std::string coin2_, std::string bank_, std::pair<int, int> min_max_, long double exchange_rate_)
            : type(type_), id(id_), seller_rating(seller_rating_), coin1(coin1_),
             coin2(coin2_), bank(bank_), min_max(min_max_), exchange_rate(exchange_rate_) {}
};

// Связка
struct Chain{
    Order buy;
    std::pair<std::string, std::string> change;
    Order sell;
    long double spread;

    Chain(Order buy_, std::pair<std::string, std::string> change_, Order sell_, long double spread_) : buy(buy_), change(change_), sell(sell_), spread(spread_) {};
};

std::vector<std::string> types {"BUY", "SELL"};
std::vector<std::string> ids {"https://www.youtube.com/", "https://docs.google.com/spreadsheets/u/0/d/1W2VigJfqsPFK12JuIj62l1kIaeJOE03xK-PTkzmMh2E/htmlview", "https://youtu.be/QMMgjjGugHE?si=PVyBYphuinGCxVun"};
std::vector<std::string> coins {"BTC", "ETH", "DOGY", "SOLU", "WLD", "COTI"};
std::vector<std::string> banks {"Tinkoff", "Sber", "Alpha", "Qiwi", "SBP"};


std::mt19937 gen;

Chain getFakeChain() {
    std::string fake_type1 = types[gen() % types.size()];
    std::string fake_id1 = ids[gen() % ids.size()];
    long double fake_rating1 = (long double)(gen() % 500) / 100;
    std::string fake_coin11 = "USDT";
    std::string fake_coin21 = coins[gen() % coins.size()];
    std::string fake_bank1 = banks[gen() % banks.size()];
    std::pair<int, int> fake_min_max1 = {gen() % 100, gen() % 1000000 + 100};
    long double fake_exchange_rate1 = (long double)(gen() % 1000000) / 100;

    Order order1(fake_type1, fake_id1, fake_rating1, fake_coin11, fake_coin21, fake_bank1, fake_min_max1, fake_exchange_rate1);

    std::string fake_type2 = types[gen() % types.size()];
    std::string fake_id2 = ids[gen() % ids.size()];
    long double fake_rating2 = (long double)(gen() % 500) / 100;
    std::string fake_coin12 = coins[gen() % coins.size()];
    std::string fake_coin22 = "USDT";
    std::string fake_bank2 = banks[gen() % banks.size()];
    std::pair<int, int> fake_min_max2 = {gen() % 100, gen() % 1000000 + 100};
    long double fake_exchange_rate2 = (long double)(gen() % 1000000) / 100;

    Order order2(fake_type2, fake_id2, fake_rating2, fake_coin12, fake_coin22, fake_bank2, fake_min_max2, fake_exchange_rate2);

    std::pair<std::string, std::string> fake_change {fake_coin21, fake_coin12};

    long double fake_spread = (long double)(gen() % 100) / 10;

    Chain fake_chain(order1, fake_change, order2, fake_spread);

    return fake_chain;
}


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