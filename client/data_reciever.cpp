#include "src/nlohmann/json.hpp"
#include "logic_fwd.hpp"
#include "logic_simulator.cpp"
#include <string>

struct DataReciever {
    // какой-то ip, какой-то сервер
    DataReciever() = default;

    Chain recieveNewChain() {
        nlohmann::json recievedJson = nlohmann::json::parse(getJsonToString());

        std::string type_1 = recievedJson["order1"]["type"].get<std::string>();
        std::string id_1 = recievedJson["order1"]["id"].get<std::string>();
        long double rating_1 = recievedJson["order1"]["seller_rating"].get<long double>();
        std::string coin1_1 = recievedJson["order1"]["coin1"].get<std::string>();
        std::string coin2_1 = recievedJson["order1"]["coin2"].get<std::string>();
        std::string bank_1 = recievedJson["order1"]["bank"].get<std::string>();
        std::pair<int, int> min_max_1 = recievedJson["order1"]["min_max"].get<std::pair<int, int>>();
        long double exchange_rate_1 = recievedJson["order1"]["exchange_rate"].get<long double>();

        Order order1(type_1, id_1, rating_1, coin1_1, coin2_1, bank_1, min_max_1, exchange_rate_1);

        std::string type_2 = recievedJson["order2"]["type"].get<std::string>();
        std::string id_2 = recievedJson["order2"]["id"].get<std::string>();
        long double rating_2 = recievedJson["order2"]["seller_rating"].get<long double>();
        std::string coin1_2 = recievedJson["order2"]["coin1"].get<std::string>();
        std::string coin2_2 = recievedJson["order2"]["coin2"].get<std::string>();
        std::string bank_2 = recievedJson["order2"]["bank"].get<std::string>();
        std::pair<int, int> min_max_2 = recievedJson["order2"]["min_max"].get<std::pair<int, int>>();
        long double exchange_rate_2 = recievedJson["order2"]["exchange_rate"].get<long double>();

        Order order2(type_2, id_2, rating_2, coin1_2, coin2_2, bank_2, min_max_2, exchange_rate_2);

        std::pair<std::string, std::string> change {coin2_1, coin1_2};

        long double spread = recievedJson["spread"].get<long double>();

        Chain chain(order1, change, order2, spread);

        return chain;
    }
};
