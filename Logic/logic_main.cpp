#include <string>
#include <unordered_map>
#include <thread>
#include <vector>
#include <cstdint> 
#include <iostream>
#include <list>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
// #include "src/nlohmann/json.hpp"

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

struct DataReceiver {
    void receive(){

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
    void send(){

    }
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
                // Скорее всего, хотим сначала прямо здесь проверить, получили ли вообще апдейт
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

struct Connection {
	boost::asio::ip::tcp::socket socket;
	boost::asio::streambuf read_buffer;
	Connection(boost::asio::io_service & io_service): socket(io_service), read_buffer() {}
	Connection(boost::asio::io_service & io_service, size_t max_buffer_size): socket( io_service ), read_buffer( max_buffer_size ) { }
};

class UsersConnectionsClient {
	boost::asio::io_service m_ioservice;
	boost::asio::ip::tcp::acceptor m_acceptor;
	std::list<Connection> m_connections;
	using con_handle_t = std::list<Connection>::iterator;

public:
	UsersConnectionsClient(): m_ioservice(), m_acceptor(m_ioservice), m_connections() {}

	void handle_read(con_handle_t con_handle, boost::system::error_code const & err, size_t bytes_transfered) {
		if(bytes_transfered > 0) {
			std::istream is(&con_handle->read_buffer);
			std::string line;
			std::getline(is, line);
			std::cout << "Message Received: " << line << std::endl; // Для дебага
            if (line == "Asking for update\n") {
				// Если получено "Asking for update\n", проверяем наличие и отправляем либо "No updates\n", либо сам апдейт

				// std::shared_ptr<std::string> response = std::make_shared<std::string>("No updates\n");
				// auto handler = boost::bind(&UsersConnectionsClient::handle_write, this, con_handle, response, boost::asio::placeholders::error);
				// boost::asio::async_write(con_handle->socket, boost::asio::buffer(*response), handler);
        	} else {
				// Остальные случаи пока не обрабатываем(есть ли он вообще?)

				std::shared_ptr<std::string> response = std::make_shared<std::string>("ping\n");
				auto handler = boost::bind(&UsersConnectionsClient::handle_write, this, con_handle, response, boost::asio::placeholders::error);
				boost::asio::async_write(con_handle->socket, boost::asio::buffer(*response), handler);
        	}
		}
		if( !err ) {
			do_async_read(con_handle);
		} else {
			std::cerr << "We had an error: " << err.message() << std::endl; // Возможно, здесь будет более умная обработка ошибок
			m_connections.erase(con_handle);
		}
	}

	void do_async_read(con_handle_t con_handle) {
		auto handler = boost::bind(&UsersConnectionsClient::handle_read, this, con_handle, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred);
		boost::asio::async_read_until(con_handle->socket, con_handle->read_buffer, "\n", handler);
	}

	void handle_write(con_handle_t con_handle, std::shared_ptr<std::string> msg_buffer, boost::system::error_code const & err) {
		if(!err) {
			std::cout << "Finished sending message\n"; // Для дебага
			if(con_handle->socket.is_open()) {
				// Всё супер - ответ написан, подключение открыто
			}
		} else { 
			std::cerr << "We had an error: " << err.message() << std::endl;  // Возможно, здесь будет более умная обработка ошибок
			m_connections.erase(con_handle);
		}
	}

	void handle_accept(con_handle_t con_handle, boost::system::error_code const & err) {
		if(!err) {
			std::cout << "Connection from: " << con_handle->socket.remote_endpoint().address().to_string() << "\n"; // Для дебага
			std::cout << "Sending message\n"; // Для дебага
			auto buff = std::make_shared<std::string>("Hello World!\n");
			auto handler = boost::bind(&UsersConnectionsClient::handle_write, this, con_handle, buff, boost::asio::placeholders::error);
			boost::asio::async_write(con_handle->socket, boost::asio::buffer(*buff), handler);
			do_async_read(con_handle);
		} else {
			std::cerr << "We had an error: " << err.message() << std::endl; // Возможно, здесь будет более умная обработка ошибок
			m_connections.erase(con_handle);
		}
		start_accept();
	}

	void start_accept() {
		auto con_handle = m_connections.emplace(m_connections.begin(), m_ioservice);
		auto handler = boost::bind(&UsersConnectionsClient::handle_accept, this, con_handle, boost::asio::placeholders::error);
		m_acceptor.async_accept(con_handle->socket, handler);
	}

	void listen(uint16_t port) {
		auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
		m_acceptor.open(endpoint.protocol());
		m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		m_acceptor.bind(endpoint);
		m_acceptor.listen();
		start_accept();
	}

	void run() {
		m_ioservice.run();
	}
};

// Запускаем в отдельном потоке из Main::run
void raise_parser_connection(std::string parser_connection_ip, std::string parser_connection_port){
    // Поднимаем клиента для подключения к парсеру:
    boost::asio::io_context client_io_context;
    ip::tcp::resolver resolver(client_io_context);
    auto endpoints = resolver.resolve(parser_connection_ip, parser_connection_port);
    ParserConnectionClient client(client_io_context, endpoints);
    client_io_context.run();
}

// Запускаем в отдельном потоке из Main::run
void raise_users_server(uint16_t users_server_port){
    // Поднимаем сервер для подключений клиентов:
    auto users_server = UsersConnectionsClient();
	users_server.listen(users_server_port);
	users_server.run();
}

struct Main {
    // DataReceiver reciever;
    // Analysis analysis;
    // DataSender sender;

    // Вероятно, оно будет не совсем так, но в упрощенном виде можно представлять что-то такое
    // void run(){
    //     std::thread t1(&DataReceiver::receive, &reciever);
    //     std::thread t2(&Analysis::analyze, &analysis);
    //     std::thread t3(&DataSender::send, &sender);
    //     std::thread t4(&DataSender::connection_handler, &sender)
    // }
};
