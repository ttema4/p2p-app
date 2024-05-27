#ifndef DATARECIEVER_H
#define DATARECIEVER_H

// checked

#include <QDebug>
#include <QException>
#include <QObject>
#include <QString>
#include <QThread>
#include <QVector>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <random>
#include <string>
#include <thread>
#include "config.h"
#include "logic_fwd.h"

using namespace boost::asio;

struct Client : QObject {
    Q_OBJECT
public:
    virtual void start() = 0;
signals:
    void error(const QString &ec);
    void dataRecieved(const QString &data);
};

struct LocalClient : Client {
public:
    LocalClient(io_context &io_context_) : cli_io_context(io_context_) {
    }

    void check_updates();
    void start() final;

    ~LocalClient();

private:
    Chain getFakeChain();
    std::string getJsonToString();

    io_context &cli_io_context;
    steady_timer check_timer{cli_io_context};
    std::mt19937 gen;

    std::thread th;
};

struct ServerClient : Client {
public:
    ServerClient(io_context &io_context_, const ip::tcp::resolver::results_type &endpoints_)
        : cli_io_context(io_context_), socket(io_context_) {
        do_connect(endpoints_);
    }

    void start() final;

    ~ServerClient();

private:
    void do_connect(const ip::tcp::resolver::results_type &endpoints);
    void send_request(const std::string &request);
    void start_check_updates();
    void start_receive();

    io_context &cli_io_context;
    ip::tcp::socket socket;
    streambuf response;
    steady_timer check_timer{cli_io_context};

    std::thread th;
};

struct DataReciever : QObject {
    Q_OBJECT
public:
    static DataReciever &getInstance();

    bool init();

    void start() {
        client->start();
    }

    ~DataReciever();

signals:
    void dataParsed(const QVector<Chain> &chains);
    void error(const QString &ec);

private slots:
    void recieveNewChain(const QString &response);

private:
    io_context cli_io_context;
    Client *client;

    DataReciever() = default;
    DataReciever(const DataReciever &) = delete;
    DataReciever &operator=(const DataReciever &) = delete;
};

#endif  // DATARECIEVER_H
