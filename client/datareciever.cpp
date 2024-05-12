#include "datareciever.h"

void LocalClient::start() {
    check_updates();
    th = std::move(std::thread([this]() { io_context.run(); }));
}

Chain LocalClient::getFakeChain() {
    static std::vector<std::string> types {"BUY", "SELL"};
    static std::vector<std::string> ids {"https://docs.google.com/spreadsheets/u/0/d/1W2VigJfqsPFK12JuIj62l1kIaeJOE03xK-PTkzmMh2E/htmlview", "https://youtu.be/QMMgjjGugHE?si=PVyBYphuinGCxVun", "https://www.youtube.com/watch?v=dQw4w9WgXcQ", "https://youtu.be/MGkWSAnoDOI?si=IyLCYPO6QFl2-b-Y&t=330"};
    static std::vector<std::string> coins {"BTC", "ETH", "DOGY", "SOLU", "WLD", "COTI"};
    static std::vector<std::string> banks {"Tinkoff", "Sber", "Alpha", "VTB", "SBP", "Raif", "Gasprom"};
    static std::vector<std::string> markets {"ByBit", "BitPapa", "Binance", "Telegram"};


    std::string fake_type1 = types[gen() % types.size()];
    std::string fake_market1 = markets[gen() % markets.size()];
    std::string fake_id1 = ids[gen() % ids.size()];
    double fake_rating1 = (double)(gen() % 500) / 100;
    std::string fake_coin11 = "USDT";
    std::string fake_coin21 = coins[gen() % coins.size()];
    std::vector<std::string> fake_bank1 = std::vector<std::string>{(banks[gen() % banks.size()])};
    std::pair<int, int> fake_min_max1 = {gen() % 100, gen() % 1000000 + 100};
    double fake_exchange_rate1 = (double)(gen() % 1000000) / 100;

    Order order1(fake_type1, fake_market1, fake_id1, fake_rating1, fake_coin11, fake_coin21, fake_bank1, fake_min_max1, fake_exchange_rate1);

    std::string fake_type2 = types[gen() % types.size()];
    std::string fake_market2 = markets[gen() % markets.size()];
    std::string fake_id2 = ids[gen() % ids.size()];
    double fake_rating2 = (double)(gen() % 500) / 100;;
    std::string fake_coin12 = "USDT";
    std::string fake_coin22 = coins[gen() % coins.size()];
    std::vector<std::string> fake_bank2 = std::vector<std::string>{(banks[gen() % banks.size()])};
    std::pair<int, int> fake_min_max2 = {gen() % 100, gen() % 1000000 + 100};
    double fake_exchange_rate2 = (double)(gen() % 1000000) / 100;

    Order order2(fake_type2, fake_market2, fake_id2, fake_rating2, fake_coin12, fake_coin22, fake_bank2, fake_min_max2, fake_exchange_rate2);

    std::pair<std::string, std::string> fake_change {fake_coin21, fake_coin22};

    double fake_spread = (double)(gen() % 2000) / 10 - 100;

    Chain fake_chain(order1, fake_change, order2, fake_spread);

    return fake_chain;
}

std::string LocalClient::getJsonToString() {
    gen();
    if (gen() % 2 == 0) {
        int n = gen() % 91 + 10;
        std::vector<Chain> chains;
        for (int i = 0; i < n; i++) chains.push_back(getFakeChain());
        nlohmann::json newjson = chains;
        return newjson.dump();
    } else {
        return "No updates\r";
    }
}

void LocalClient::check_updates() {
    emit dataRecieved(QString::fromStdString(getJsonToString()));
    check_timer.expires_after(std::chrono::seconds(CHECK_UPDATE_INTERVAL));
    check_timer.async_wait([this](boost::system::error_code ec) {
        if (!ec) {
            check_updates();
        } else {
            emit error(QString("Ошибка во время приема") + QString::fromStdString(ec.what()));
        }
    });
}

LocalClient::~LocalClient() {
    io_context.stop();
    if (th.joinable()) th.join();
}



void ServerClient::start() {
    start_receive();
    start_check_updates();
    th = std::move(std::thread([this]() { io_context.run(); }));
}

void ServerClient::start_check_updates() {
    check_timer.expires_after(std::chrono::seconds(CHECK_UPDATE_INTERVAL));
    check_timer.async_wait([this](boost::system::error_code ec) {
        if (!ec) {
            send_request("need update");
            start_check_updates(); // Повторно запускаем проверку обновлений
        }
    });
}

void ServerClient::send_request(const std::string& request) {
    std::cout << "Sending request: " << request << std::endl;
    async_write(socket, boost::asio::buffer(request + "\n"), [this](boost::system::error_code ec, std::size_t /*length*/) {
        if (ec) {
            emit error(QString("Error sending request: ") + QString::fromStdString(ec.message()));
        }
    });
}

void ServerClient::start_receive() {
    async_read_until(socket, response, '\n', [this](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            std::istream response_stream(&response);
            std::string response;
            std::getline(response_stream, response);
            emit dataRecieved(QString::fromStdString(response));
            start_receive();
        } else {
            emit error(QString("Error receiving response: ") + QString::fromStdString(ec.message()));
        }
    });
}

void ServerClient::do_connect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
    boost::system::error_code ec;
    boost::asio::connect(socket, endpoints, ec);
    if (ec) {
        qDebug() << ec.what();
        throw QException();
    }
}

ServerClient::~ServerClient() {
    io_context.stop();
    if (th.joinable()) th.join();
}



DataReciever& DataReciever::getInstance() {
    static DataReciever instance;
    return instance;
}

bool DataReciever::init() {
    if (USE_SERVER) {
        try {
            boost::asio::ip::tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(SERVER_IP, std::to_string(SERVER_PORT));
            qDebug() << "SUCCESS";
            client = new ServerClient(io_context, endpoints);
            qDebug() << "Успешное подключение";
        } catch (QException e) {
            qDebug() << "Произошла ошибка во время работы сервер: " << e.what();
            return false;
        }
    } else {
        client = new LocalClient(io_context);
    }
    connect(client, &Client::error, [](QString ec) { qDebug() << ec; });
    connect(client, &Client::dataRecieved, this, &DataReciever::recieveNewChain);
    return true;
}

void DataReciever::recieveNewChain(QString str) {
    if (str != "No updates\r" && str != "Hello World!") {
        qDebug() << str;
        QVector<Chain> chains = nlohmann::json::parse(str.toStdString()).get<QVector<Chain>>();
        emit dataParsed(chains);
    } else {
        // qDebug() << str;
    }
}

DataReciever::~DataReciever() {
    delete client;
}
