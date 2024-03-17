#include <string>
#include <unordered_map>
#include <thread>
#include <vector>
#include <boost/asio.hpp>

using namespace boost::asio;

struct Order {
    std::string type;
    std::string id;
    long double seller_rating;
    std::string coin1;
    std::string coin2;
    std::vector<std::string> banks;
    std::pair<int, int> min_max;
    long double exchange_rate;
    Order(std::string type_, std::string id_, long double seller_rating_, std::string coin1_,
            std::string coin2_, std::vector<std::string> banks_, std::pair<int, int> min_max_, long double exchange_rate_)
            : type(type_), id(id_), seller_rating(seller_rating_), coin1(coin1_),
             coin2(coin2_), banks(banks_), min_max(min_max_), exchange_rate(exchange_rate_) {}
};

struct Orders{
    std::vector<Order> list;
    Orders() = default;
};

struct MarketRates{
    std::unordered_map<std::string, std::vector<std::pair<std::string, long double>>> list;
    MarketRates() = default;
};

struct Chain{
    Order buy;
    std::pair<std::string, std::string> change;
    Order sell;
    long double spread;
    
    Chain(Order buy_, const std::pair<std::string, std::string> change_, Order sell_, long double spread_)
    : buy(buy_), change(change_), sell(sell_), spread(spread_) {}
    
};

struct Chains{
    std::vector<Chain> list;
    Chains() = default;
};

class ParserConnectionClient {
public:
    ParserConnectionClient(boost::asio::io_context& io_context, const ip::tcp::resolver::results_type& endpoints)
        : io_context_(io_context), socket_(io_context) {
        do_connect(endpoints);
    }

    void start() {
        start_receive();
        start_check_updates();
    }

private:
    void start_check_updates() {
        check_timer.expires_after(std::chrono::seconds(1)); // Проверяем обновления каждую секунду
        check_timer.async_wait([this](boost::system::error_code ec) {
            if (!ec) {
                send_request("Asking for update\n"); // Запрос, по которому парсер присылает обновление, либо говорит о его отсутствии
                start_check_updates();
            }
        });
    }

    void send_request(const std::string& request) {
        std::cout << "Sending request: " << request << std::endl; // Для дебага
        async_write(socket_, buffer(request + "\n"), [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (ec) {
                std::cerr << "Error sending request: " << ec.message() << std::endl; // Возможно, здесь будет более умная обработка ошибок
            }
        });
    }

    void start_receive() {
        async_read_until(socket_, response_, '\n', [this](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::istream response_stream(&response_);
                std::string response;
                std::getline(response_stream, response);
                std::cout << "Received response: " << response << std::endl; // Для дебага
                // |
                // |
                // V
                // response - наши данные, которые мы получили
                // Здесь кладем в очередь / передаем во внешний метод - этот этап в работе...
                start_receive();
            } else {
                std::cerr << "Error receiving response: " << ec.message() << std::endl; // Возможно, здесь будет более умная обработка ошибок
            }
        });
    }

    void do_connect(const ip::tcp::resolver::results_type& endpoints) {
        async_connect(socket_, endpoints, [this](boost::system::error_code ec, ip::tcp::endpoint) {
            if (!ec) {
                std::cout << "Connected to server." << std::endl; // Для дебага
                start();
            } else {
                std::cerr << "Error connecting to server: " << ec.message() << std::endl; // Возможно, здесь будет более умная обработка ошибок
            }
        });
    }

private:
    boost::asio::io_context& io_context_;
    ip::tcp::socket socket_;
    boost::asio::streambuf response_;
    boost::asio::steady_timer check_timer{io_context_};
};

struct DataReceiver {
    void receive(){
        // Метод для подключения к парсеру
        try {
            boost::asio::io_context io_context;
            ip::tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve("localhost", "12345"); // Временный сокет
            ParserConnectionClient client(io_context, endpoints);
            io_context.run();
        } catch (std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl; // Возможно, здесь будет более умная обработка ошибок
        }
    }
};

struct Analysis {
    void analyze(Chains& chains, Orders& orders_for_buy, Orders& orders_for_sell, MarketRates& market_rates){
        std::unordered_map<std::string, std::vector<Order>> sell_by_coin;
        for(Order sell : orders_for_sell.list){
            sell_by_coin[sell.coin1].push_back(sell);
        }
        for (Order buy : orders_for_buy.list){
            std::string coin_buy = buy.coin2;
            for (std::pair<std::string, long double> market : market_rates.list[coin_buy]){
                std::string coin_sell = market.first;
                long double rate = market.second;
                for (Order sell : sell_by_coin[coin_sell]){
                    long double spread = ((sell.exchange_rate / buy.exchange_rate) - 1) * 100;
                    chains.list.push_back(Chain(buy, std::make_pair(coin_buy, coin_sell), sell, spread));
                }
            }
        }
    }
};

struct DataSender {
    void connection_handler(){

    }

    void send(){

    }
};

struct Main {
    DataReceiver reciever;
    Analysis analysis;
    DataSender sender;

    MarketRates market_rates;
    Orders orders_for_buy;
    Orders orders_for_sell;
    Chains chains;    

    // Вероятно, оно будет не совсем так, но в упрощенном виде можно представлять что-то такое
    // void run(){
    //     std::thread t1(&DataReceiver::receive, &reciever);
    //     std::thread t2(&Analysis::analyze, &analysis);
    //     std::thread t3(&DataSender::send, &sender);
    //     std::thread t4(&DataSender::connection_handler, &sender)
    // }
};
