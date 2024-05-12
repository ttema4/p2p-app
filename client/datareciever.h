#ifndef DATARECIEVER_H
#define DATARECIEVER_H

#include "lib/nlohmann/json.hpp"
#include <boost/asio.hpp>
#include "logic_fwd.h"
#include "config.h"
#include <QObject>
#include <QException>
#include <QThread>
#include <QVector>
#include <QString>
#include <QDebug>


#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <thread>

struct Client : QObject {
    Q_OBJECT
public:
    virtual void start() = 0;
signals:
    void error(QString ec);
    void dataRecieved(QString data);
};


struct LocalClient : Client {
public:
    LocalClient(boost::asio::io_context& io_context_) : io_context(io_context_) {}
    void check_updates();
    void start() override;

    ~LocalClient();

private:
    Chain getFakeChain();
    std::string getJsonToString();

    boost::asio::io_context& io_context;
    boost::asio::steady_timer check_timer{io_context};
    std::mt19937 gen;
    std::thread th;
};


struct ServerClient : Client {
public:
    ServerClient(boost::asio::io_context& io_context_, const boost::asio::ip::tcp::resolver::results_type& endpoints_)
        : io_context(io_context_), socket(io_context_) {
        do_connect(endpoints_);
    }

    void start() override;

    ~ServerClient();

private:
    void do_connect(const boost::asio::ip::tcp::resolver::results_type& endpoints);
    void send_request(const std::string& request);
    void start_check_updates();
    void start_receive();

    boost::asio::io_context& io_context;
    boost::asio::ip::tcp::socket socket;
    boost::asio::streambuf response;
    boost::asio::steady_timer check_timer{io_context};
    std::thread th;
};


struct DataReciever : QObject {
    Q_OBJECT
public:
    static DataReciever& getInstance();
    bool init();
    void start() {
        client->start();
    }
    ~DataReciever();

signals:
    void dataParsed(QVector<Chain> chains);

private slots:
    void recieveNewChain(QString str);

private:
    boost::asio::io_context io_context;
    Client *client;

    DataReciever() = default;
    DataReciever(const DataReciever&) = delete;
    DataReciever& operator=(const DataReciever&) = delete;
};


#endif // DATARECIEVER_H
