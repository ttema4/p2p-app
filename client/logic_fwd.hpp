#ifndef LOGIC_HPP_
#define LOGIC_HPP_

#include <string>
#include <vector>
#include <random>
#include <iostream>

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

#endif // LOGIC_HPP_