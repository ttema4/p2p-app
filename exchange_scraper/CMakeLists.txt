cmake_minimum_required(VERSION 3.29.2)

option(BUILD_TESTS "tests" OFF)

if(BUILD_TESTS)
    add_executable(markets_scraper markets.cpp scraper.cpp tests/bybit\ simulator.cpp)
else()
    add_executable(markets_scraper markets.cpp scraper.cpp main.cpp)
    target_include_directories(markets_scraper PRIVATE "/usr/local/include")
endif()

set_target_properties(
    markets_scraper PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)