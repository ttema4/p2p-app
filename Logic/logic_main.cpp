#include <string>
#include <unordered_map>
#include <thread>
#include <vector>
#include <boost/asio.hpp>

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

struct DataReceiver {

    size_t read_complete(char* buf, const boost::system::error_code & err, size_t bytes){
        if(err){
            return 0;
        }
        bool found = std::find(buf, buf + bytes, '\n') < buf + bytes; // Вместо '\n' здесь любой флаг конца сообщения
        return found ? 0 : 1;
    }

    void process_new_data(std::string data){
        // Здесь разбираем полученную строку в Orders и тд
    }

    void ask_for_update(){
    // В финальной имплементации здесь будет бОльшая часть проверок а-ля "подключение оборвалось" и всё такое прочее
        boost::asioio_service service;
        std::string message = "asking for update\n";
        ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001); // Порт временный
        ip::tcp::socket sock(service);
        sock.connect(ep);
        bool no_updates = true;
        while(no_updates){
            sock.write_some(buffer(messasge));
            char answer_buff[1024]; // Размер буффера временный
            int bytes = read(sock, buffer(answer_buff), boost::bind(read_complete,answer_buff,_1,_2));
            std::string copy(answer_buff, bytes - 1);
            if(copy != "no updates"){
                no_updates = false;
            }
        }
        sock.close();
        process_new_data(copy);
    }

    void receive(){
        while(true){ // Наверно, наивно, но пока как есть ))
            ask_for_update();
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
