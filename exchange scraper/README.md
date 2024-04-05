FOR TESTS:

1. bybit simulator: clang++ -std=c++17 exchanges.hpp scraper.hpp nlohmann/json.hpp magic_enum/magic_enum.hpp exchanges.cpp scraper.cpp tests/'bybit simulator.cpp'



FOR PODUCTION:

1. clang++ -std=c++17 exchanges.hpp scraper.hpp nlohmann/json.hpp magic_enum/magic_enum.hpp exchanges.cpp scraper.cpp main.cpp