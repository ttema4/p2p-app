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
    std::vector<std::unique_ptr<Order>> list;
    Orders() = default;
};

// Обертка
struct MarketRates{
    // Проблемка
    // Pair не может быть ключом, нужен либо кастом хэш, либо другое хранение...
    std::unordered_map<std::pair<std::string, std::string>, long double> list; // Курсы по маркету
    MarketRates() = default;
};

// Связка
struct Chain{
    Order buy;
    std::pair<std::string, std::string> change;
    Order sell;
    long double spread;
};

// Обертка для хранения уже посчитанных связок
struct Chains{
    std::vector<std::unique_ptr<Chain>> list;
};

// std::queue<std::unique_ptr<Orders>> queue;

class DataReceiver {
    // Получить - разложить в orders и market_rates
    // Вытаскиваем из БД? Получаем .json через TCP? Вытаскиваем из очереди Rabbit'а?
    void receive(){
        
    }
};

class Analysis {
    // Здесь все методы для обработки поллученных данных до состояния Result
    // Вытаскиваем order'a(нужно умное хранение в каком то определенном порядке)
    // Проходимся for'ами
    // Складываем в Chains в каком то виде Order 1 - Market - Order 2 - Spread

};

class DataSender {
    // Вытаскиваем из Chains - отправляем
    // Кладем в БД? Отправляем .json через TCP? Кладем в очередь Rabbit'а?
    // Handle подключения клиентов
    // Вектор клиентов - пройтись - отправить
};

class Main {
    DataReceiver reciever;
    Analysis analysis;
    DataSender sender;

    void run(){
        
    }
};
