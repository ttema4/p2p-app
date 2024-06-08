#ifndef PARSER_CONNECTION_CLIENT_HPP_
#define PARSER_CONNECTION_CLIENT_HPP_

#include <boost/asio.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace boost::asio;

namespace p2p {

class ParserConnectionClient {
public:
    ParserConnectionClient(boost::asio::io_context &io_context, const ip::tcp::resolver::results_type &endpoints);

    void start();

private:
    void start_check_updates();
    void send_request(const std::string &request);
    void start_receive();
    void do_connect(const ip::tcp::resolver::results_type &endpoints);
    void do_reconnect();

private:
    boost::asio::io_context &io_context_;
    ip::tcp::socket socket_;
    boost::asio::streambuf response_;
    boost::asio::steady_timer check_timer{io_context_};
    ip::tcp::resolver::results_type endpoints_;
};

}  // namespace p2p

#endif  // PARSER_CONNECTION_CLIENT_HPP_
