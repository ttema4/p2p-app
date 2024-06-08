#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../storage_structures.hpp"
#include "../json_tools.hpp"
#include "../data_processing.hpp"
#include "../logic_manager.hpp"
#include "../parser_connection_client.hpp"
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace p2p;
using namespace boost::asio;

// COMPILE THIS TESTS WITH clang++ -std=c++17 test_main.cpp
// ../logic_manager.cpp ../parser_connection_client.cpp ../users_connections_server.cpp
// ../json_tools.cpp ../data_processing.cpp -L/opt/homebrew/opt/boost/lib -I/opt/homebrew/opt/boost/include

TEST_CASE("Testing Order structure") {
    Order order("market", "buy", "link", 5.0, "USD", "BTC", {"bank1", "bank2"}, {100.0, 1000.0}, 50000.0);

    CHECK(order.market == "market");
    CHECK(order.type == "buy");
    CHECK(order.link == "link");
    CHECK(order.seller_rating == 5.0);
    CHECK(order.coin1 == "USD");
    CHECK(order.coin2 == "BTC");
    CHECK(order.banks.size() == 2);
    CHECK(order.banks[0] == "bank1");
    CHECK(order.banks[1] == "bank2");
    CHECK(order.min_max.first == 100.0);
    CHECK(order.min_max.second == 1000.0);
    CHECK(order.exchange_rate == 50000.0);
}

TEST_CASE("Testing Orders structure") {
    Orders orders;
    orders.list.push_back(Order("market", "buy", "link", 5.0, "USD", "BTC", {"bank1"}, {100.0, 1000.0}, 50000.0));

    CHECK(orders.list.size() == 1);
    CHECK(orders.list[0].market == "market");
    CHECK(orders.list[0].type == "buy");
}

TEST_CASE("Testing MarketRates structure") {
    MarketRates market_rates;
    market_rates.list["market"]["BTC"].push_back({"USD", 50000.0});

    CHECK(market_rates.list.size() == 1);
    CHECK(market_rates.list["market"].size() == 1);
    CHECK(market_rates.list["market"]["BTC"].size() == 1);
    CHECK(market_rates.list["market"]["BTC"][0].first == "USD");
    CHECK(market_rates.list["market"]["BTC"][0].second == 50000.0);
}

TEST_CASE("Testing Chain structure") {
    Order buy("market", "buy", "link1", 5.0, "USD", "BTC", {"bank1"}, {100.0, 1000.0}, 50000.0);
    Order sell("market", "sell", "link2", 4.5, "BTC", "USD", {"bank2"}, {100.0, 1000.0}, 51000.0);
    Chain chain(buy, {"USD", "BTC"}, sell, 2.0);

    CHECK(chain.buy.market == "market");
    CHECK(chain.change.first == "USD");
    CHECK(chain.change.second == "BTC");
    CHECK(chain.sell.market == "market");
    CHECK(chain.spread == 2.0);
}

TEST_CASE("Testing Chains structure") {
    Chains chains;
    Order buy("market", "buy", "link1", 5.0, "USD", "BTC", {"bank1"}, {100.0, 1000.0}, 50000.0);
    Order sell("market", "sell", "link2", 4.5, "BTC", "USD", {"bank2"}, {100.0, 1000.0}, 51000.0);
    Chain chain(buy, {"USD", "BTC"}, sell, 2.0);
    chains.list.push_back(chain);

    CHECK(chains.list.size() == 1);
    CHECK(chains.list[0].buy.market == "market");
    CHECK(chains.list[0].change.first == "USD");
    CHECK(chains.list[0].change.second == "BTC");
}

TEST_CASE("Testing SharedString class") {
    SharedString shared_string("initial");

    CHECK(shared_string.get() == "initial");

    shared_string.set("new_value");
    CHECK(shared_string.get() == "new_value");

    std::thread t1([&]() {
        for (int i = 0; i < 100; ++i) {
            shared_string.set("thread1_" + std::to_string(i));
        }
    });

    std::thread t2([&]() {
        for (int i = 0; i < 100; ++i) {
            shared_string.set("thread2_" + std::to_string(i));
        }
    });

    t1.join();
    t2.join();

    std::string result = shared_string.get();
    CHECK((result.find("thread1_") != std::string::npos || result.find("thread2_") != std::string::npos));
}

TEST_CASE("Testing global variables initialization") {
    CHECK(PARSER_IP.empty());
    CHECK(PARSER_PORT == 0);
    CHECK(USERS_SERVER_PORT == 0);
    CHECK(!PARSER_LOGS_ON);
    CHECK(!PARSER_RESPONSE_LOGS_ON);
    CHECK(!USERS_SERVER_LOGS_ON);
    CHECK(!ORDERS_FOR_BUY_LOGS_ON);
    CHECK(!ORDERS_FOR_SELL_LOGS_ON);
    CHECK(!SELL_BY_COIN_LOGS_ON);
    CHECK(!MARKET_RATES_LOGS_ON);
    CHECK(!CHAINS_LOGS_ON);

    CHECK(last_version_by_user.empty());
    CHECK(markets.empty());
    CHECK(parsers_responses.size_approx() == 0);
    CHECK(up_to_date_version.get().empty());
}

TEST_CASE("Testing fix_banks_naming_and_filter") {
    Orders orders;
    orders.list.push_back(Order("market1", "type1", "link1", 4.5, "coin1", "coin2", {"A-Bank", "Sberbank", "NN", "Raiffeisenbank"}, {1.0, 10.0}, 1.2));
    orders.list.push_back(Order("market2", "type2", "link2", 3.2, "coin3", "coin4", {"Tinkoff", "FPS"}, {0.5, 5.0}, 0.8));

    fix_banks_naming_and_filter(orders);

    CHECK(orders.list.size() == 2);
    CHECK(orders.list[0].banks.size() == 3);
    CHECK(orders.list[0].banks[0] == "Alpha");
    CHECK(orders.list[0].banks[1] == "Sber");
    CHECK(orders.list[0].banks[2] == "Raif");
    CHECK(orders.list[1].banks.size() == 2);
    CHECK(orders.list[1].banks[0] == "Tinkoff");
    CHECK(orders.list[1].banks[1] == "SBP");
}

TEST_CASE("Testing unpack_json") {
    std::string parsers_response = R"({
        "orders": [
            {
                "market": "market1",
                "direction": "type1",
                "link": "link1",
                "currency": "coin1",
                "coin": "coin2",
                "lower_limit": "1.0",
                "upper_limit": "10.0",
                "price": "1.2",
                "payment_methods": ["A-Bank", "Sberbank"]
            }
        ],
        "spot_rates": [
            {
                "market": "market1",
                "coin1/coin2": "1.5"
            }
        ]
    })";

    Orders orders;
    MarketRates market_rates;
    unpack_json(parsers_response, orders, market_rates);

    CHECK(orders.list.size() == 1);
    CHECK(orders.list[0].market == "market1");
    CHECK(orders.list[0].type == "type1");
    CHECK(orders.list[0].link == "link1");
    CHECK(orders.list[0].coin1 == "coin1");
    CHECK(orders.list[0].coin2 == "coin2");
    CHECK(orders.list[0].min_max.first == 1.0);
    CHECK(orders.list[0].min_max.second == 10.0);
    CHECK(orders.list[0].exchange_rate == 1.2);
    CHECK(orders.list[0].banks.size() == 2);
    CHECK(orders.list[0].banks[0] == "Alpha");
    CHECK(orders.list[0].banks[1] == "Sber");

    CHECK(market_rates.list["market1"]["coin1"][0].first == "coin2");
    CHECK(market_rates.list["market1"]["coin1"][0].second == 1.5);
    CHECK(market_rates.list["market1"]["coin2"][0].first == "coin1");
    CHECK(market_rates.list["market1"]["coin2"][0].second == doctest::Approx(1 / 1.5).epsilon(0.0001));
}

TEST_CASE("Testing pack_json") {
    Chains chains;
    Order buy_order("market1", "buy", "link1", 4.5, "coin1", "coin2", {"Alpha"}, {1.0, 10.0}, 1.2);
    Order sell_order("market2", "sell", "link2", 3.2, "coin3", "coin4", {"Sber"}, {0.5, 5.0}, 0.8);
    Chain chain(buy_order, {"coin1", "coin3"}, sell_order, 1.5);
    chains.list.push_back(chain);

    std::string json_str = pack_json(chains);
    nlohmann::json j = nlohmann::json::parse(json_str);

    CHECK(j.size() == 1);
    CHECK(j[0]["buy"]["market"] == "market1");
    CHECK(j[0]["buy"]["type"] == "buy");
    CHECK(j[0]["buy"]["id"] == "link1");
    CHECK(j[0]["buy"]["coin1"] == "coin1");
    CHECK(j[0]["buy"]["coin2"] == "coin2");
    CHECK(j[0]["buy"]["banks"][0] == "Alpha");
    CHECK(j[0]["buy"]["exchange_rate"] == 1.2);
    CHECK(j[0]["change"][0] == "coin1");
    CHECK(j[0]["change"][1] == "coin3");
    CHECK(j[0]["sell"]["market"] == "market2");
    CHECK(j[0]["sell"]["type"] == "sell");
    CHECK(j[0]["sell"]["id"] == "link2");
    CHECK(j[0]["sell"]["coin1"] == "coin3");
    CHECK(j[0]["sell"]["coin2"] == "coin4");
    CHECK(j[0]["sell"]["banks"][0] == "Sber");
    CHECK(j[0]["sell"]["exchange_rate"] == 0.8);
    CHECK(j[0]["spread"] == 1.5);
}

TEST_CASE("Testing data processing") {
    std::ifstream file("mock_data.json");
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string data_str = buffer.str();
    Analysis analysis;
    Orders orders;
    MarketRates market_rates;
    unpack_json(data_str, orders, market_rates);
    Chains chains = analysis.analyze(orders, market_rates);
    CHECK(chains.list.size() > 0);
}
