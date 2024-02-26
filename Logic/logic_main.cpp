#include <string>
#include <unordered_map>
#include <thread>

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

// Обертка
struct Orders{
    // ПРОБЛЕМА
    // !!! Нужно более умное хранение / умная соротировка, чтобы быстро находить ордер по банку/монетам,
    // а не проходить каждый раз весь вектор. !!!
    std::vector<std::shared_ptr<Order>> list;
    Orders() = default;
};

// Обертка
struct MarketRates{
    std::unordered_map<std::string, std::vector<std::pair<std::string, long double>>> list; // Курсы по маркету
    MarketRates() = default;
};

// Связка
struct Chain{
    std::shared_ptr<Order> buy;
    std::pair<std::string, std::string> change;
    std::shared_ptr<Order> sell;
    long double spread;
    
    Chain(std::shared_ptr<Order> buy_, const std::pair<std::string, std::string> change_, std::shared_ptr<Order> sell_, long double spread_)
    : buy(buy_), change(change_), sell(sell_), spread(spread_) {}
    
};

// Обертка для хранения уже посчитанных связок
struct Chains{
    std::vector<std::shared_ptr<Chain>> list;
    Chains() = default;
};

// std::queue<std::unique_ptr<Orders>> queue; - нужна потокобезопасная

struct DataReceiver {
    // Получить - разложить в orders и market_rates
    // Вытаскиваем из БД? Получаем .json через TCP? Вытаскиваем из очереди Rabbit'а?
    void receive(){
        
    }
};

struct Analysis {
    // Здесь все методы для обработки поллученных данных до состояния Result
    // Вытаскиваем order'a(нужно умное хранение в каком то определенном порядке)
    // Проходимся for'ами
    // Складываем в Chains в каком то виде Order 1 - Market - Order 2 - Spread
    void analyze(Chains& chains, Orders& orders_for_buy, Orders& orders_for_sell, MarketRates& market_rates){
        // Первая версия не учитыыает требование потокобезопасности каких либо структур
        // Поэтому просто пройдемся по всем ордерам и по всем курсам и сделаем связки
        std::unordered_map<std::string, std::vector<std::shared_ptr<Order>>> sell_by_coin;
        for(std::shared_ptr<Order> sell : orders_for_sell.list){
            sell_by_coin[sell->coin1].push_back(sell);
        }
        for (std::shared_ptr<Order> buy : orders_for_buy.list){
            std::string coin_buy = buy->coin2;
            for (std::pair<std::string, long double> market : market_rates.list[coin_buy]){
                std::string coin_sell = market.first;
                long double rate = market.second;
                for (std::shared_ptr<Order> sell : sell_by_coin[coin_sell]){
                    long double spread = ((sell->exchange_rate / buy->exchange_rate) - 1) * 100;
                    chains.list.push_back(std::make_shared<Chain>(buy, std::make_pair(coin_buy, coin_sell), sell, spread));
                }
            }
        }
    }

};

struct DataSender {
    // Вытаскиваем из Chains - отправляем
    // Кладем в БД? Отправляем .json через TCP? Кладем в очередь Rabbit'а?
    // Handle подключения клиентов
    // Вектор клиентов - пройтись - отправить
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
    // }
};
