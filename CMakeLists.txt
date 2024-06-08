cmake_minimum_required(VERSION 3.28)

project(p2p
    DESCRIPTION "The p2p analizer"
    LANGUAGES CXX
)

add_subdirectory(client)
add_subdirectory(server_logic)
add_subdirectory(exchange_scraper)
