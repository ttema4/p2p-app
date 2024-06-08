
#include <thread>

#include "logic_manager.hpp"

int main() {
    p2p::apply_config();
    std::thread t1(&p2p::raise_parser_connection_and_users_server);
    std::thread t2(&p2p::to_analysis);
    t1.join();
    t2.join();
}