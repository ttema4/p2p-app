#include "datareciever.h"

// checked

// LocalClient
void LocalClient::start() {
    check_updates();
    th = std::move(std::thread([this]() { cli_io_context.run(); }));
}

Chain LocalClient::getFakeChain() {
    static std::vector<std::string> types{"BUY", "SELL"};
    static std::vector<std::string> ids{
        "https://docs.google.com/spreadsheets/u/0/d/1W2VigJfqsPFK12JuIj62l1kIaeJOE03xK-PTkzmMh2E/htmlview",
        "https://youtu.be/QMMgjjGugHE?si=PVyBYphuinGCxVun", "https://www.youtube.com/watch?v=dQw4w9WgXcQ",
        "https://youtu.be/MGkWSAnoDOI?si=IyLCYPO6QFl2-b-Y&t=330"
    };
    static std::vector<std::string> coins{"BTC", "ETH", "DOGY", "SOLU", "WLD", "COTI"};
    static std::vector<std::string> banks{"Tinkoff", "Sber", "Alpha", "VTB", "SBP", "Raif", "Gasprom"};
    static std::vector<std::string> markets{"ByBit", "Bitget"};

    std::string fake_type1 = types[gen() % types.size()];
    std::string fake_market1 = markets[gen() % markets.size()];
    std::string fake_id1 = ids[gen() % ids.size()];
    double fake_rating1 = static_cast<double>(gen() % 500) / 100;
    std::string fake_coin11 = "USDT";
    std::string fake_coin21 = coins[gen() % coins.size()];
    std::vector<std::string> fake_bank1 = std::vector<std::string>{(banks[gen() % banks.size()])};
    std::pair<int, int> fake_min_max1 = {gen() % 100, gen() % 1000000 + 100};
    double fake_exchange_rate1 = static_cast<double>(gen() % 1000000) / 100;

    Order order1(
        fake_type1, fake_market1, fake_id1, fake_rating1, fake_coin11, fake_coin21, fake_bank1, fake_min_max1,
        fake_exchange_rate1
    );

    std::string fake_type2 = types[gen() % types.size()];
    std::string fake_market2 = markets[gen() % markets.size()];
    std::string fake_id2 = ids[gen() % ids.size()];
    double fake_rating2 = static_cast<double>(gen() % 500) / 100;
    ;
    std::string fake_coin12 = "USDT";
    std::string fake_coin22 = coins[gen() % coins.size()];
    std::vector<std::string> fake_bank2 = std::vector<std::string>{(banks[gen() % banks.size()])};
    std::pair<int, int> fake_min_max2 = {gen() % 100, gen() % 1000000 + 100};
    double fake_exchange_rate2 = static_cast<double>(gen() % 1000000) / 100;

    Order order2(
        fake_type2, fake_market2, fake_id2, fake_rating2, fake_coin12, fake_coin22, fake_bank2, fake_min_max2,
        fake_exchange_rate2
    );

    std::pair<std::string, std::string> fake_change{fake_coin21, fake_coin22};
    double fake_spread = static_cast<double>(gen() % 2000) / 10 - 100;

    Chain fake_chain(order1, fake_change, order2, fake_spread);

    return fake_chain;
}

std::string LocalClient::getJsonToString() {
    gen();
    if (gen() % 2 == 0) {
        int n = gen() % 91 + 10;
        std::vector<Chain> chains;
        for (int i = 0; i < n; i++) {
            chains.push_back(getFakeChain());
        }
        return nlohmann::json(chains).dump();
    }
    return "No updates\r";
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
    cli_io_context.stop();
    if (th.joinable()) {
        th.join();
    }
}

// ServerClient
void ServerClient::do_connect(const boost::asio::ip::tcp::resolver::results_type &endpoints) {
    boost::system::error_code ec;
    boost::asio::connect(socket, endpoints, ec);
    if (ec) {
        qDebug() << "Error while connecting to server:" << ec.what();
        throw QException();
    }
    qDebug() << "Successfully connected to server";
}

void ServerClient::start() {
    start_receive();
    start_check_updates();
    th = std::move(std::thread([this]() { cli_io_context.run(); }));
}

void ServerClient::start_check_updates() {
    check_timer.expires_after(std::chrono::seconds(CHECK_UPDATE_INTERVAL));
    check_timer.async_wait([this](boost::system::error_code ec) {
        if (!ec) {
            send_request("need update");
            start_check_updates();
        }
    });
}

void ServerClient::send_request(const std::string &request) {
    qDebug() << "Sending request: " << request;
    async_write(
        socket, boost::asio::buffer(request + "\n"),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (ec) {
                qDebug() << "Error while sending request:" << QString::fromStdString(ec.message());
                emit error(QString("Error sending request: ") + QString::fromStdString(ec.message()));
            }
        }
    );
}

void ServerClient::start_receive() {
    async_read_until(socket, response, '\n', [this](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            std::istream response_stream(&response);
            std::string response;
            std::getline(response_stream, response);
            qDebug() << "Recieved response with len" << QString::number(response.size())
                     << "First 10:" << QString::fromStdString(response).left(10);
            emit dataRecieved(QString::fromStdString(response));
            start_receive();
        } else {
            qDebug() << "Error while recieving response:" << QString::fromStdString(ec.message());
            emit error(QString("Error while receiving response: ") + QString::fromStdString(ec.message()));
        }
    });
}

ServerClient::~ServerClient() {
    cli_io_context.stop();
    if (th.joinable()) {
        th.join();
    }
}

// DataReciever
DataReciever &DataReciever::getInstance() {
    static DataReciever instance;
    return instance;
}

bool DataReciever::init() {
    if (USE_SERVER) {
        try {
            ip::tcp::resolver resolver(cli_io_context);
            auto endpoints = resolver.resolve(SERVER_IP, std::to_string(SERVER_PORT));
            client = new ServerClient(cli_io_context, endpoints);
        } catch (QException e) {
            return false;
        }
    } else {
        client = new LocalClient(cli_io_context);
    }
    qDebug() << "Successfully init DataReciever";
    connect(client, &Client::error, this, &DataReciever::error);
    connect(client, &Client::dataRecieved, this, &DataReciever::recieveNewChain);
    return true;
}

void DataReciever::recieveNewChain(const QString &response) {
    if (response != "No updates\r" && response != "Hello World!") {
        QVector<Chain> chains = nlohmann::json::parse(response.toStdString()).get<QVector<Chain>>();
        emit dataParsed(chains);
    }
}

DataReciever::~DataReciever() {
    delete client;
}
