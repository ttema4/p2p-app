#include <chrono>
#include <functional>
#include <immintrin.h>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <thread>
#include <vector>

sqlite3 *stock_market_data;
int exitt = 0;
char *messaggeError;

void order_generater(std::function<void(void)> func, unsigned int interval) {
  std::thread([func, interval]() {
    exitt = sqlite3_open("stock_market_data.db", &stock_market_data);
    std::string request = "CREATE TABLE stock_market_data ("
                          "id BIGINT NOT NULL PRIMARY KEY,"
                          "fromm VARCHAR(32) NOT NULL,"
                          "too VARCHAR(32) NOT NULL,"
                          "price REAL NOT NULL,"
                          "payment_method VARCHAR(64) NOT NULL,"
                          "lower_limit REAL NOT NULL,"
                          "upper_limit REAL NOT NULL,"
                          "available REAL NOT NULL,"
                          "rating BIGINT NOT NULL,"
                          "link VARCHAR(256)"
                          ");";
    exitt = sqlite3_exec(stock_market_data, request.c_str(), NULL, 0,
                         &messaggeError);
    sqlite3_free(messaggeError);
    request = "INSERT INTO stock_market_data (id, fromm, too, price, "
              "payment_method, lower_limit, upper_limit, available, rating)"
              "VALUES (1, 'rub', 'usdt', -1, '', -1, -1, -1, -1);";
    exitt = sqlite3_exec(stock_market_data, request.c_str(), NULL, 0,
                         &messaggeError);
    sqlite3_free(messaggeError);
    request = "INSERT INTO stock_market_data (id, fromm, too, price, "
              "payment_method, lower_limit, upper_limit, available, rating)"
              "VALUES (2, 'rub', 'usdt', -1, '', -1, -1, -1, -1);";
    exitt = sqlite3_exec(stock_market_data, request.c_str(), NULL, 0,
                         &messaggeError);
    sqlite3_free(messaggeError);
    request = "INSERT INTO stock_market_data (id, fromm, too, price, "
              "payment_method, lower_limit, upper_limit, available, rating)"
              "VALUES (3, 'rub', 'usdt', -1, '', -1, -1, -1, -1);";
    exitt = sqlite3_exec(stock_market_data, request.c_str(), NULL, 0,
                         &messaggeError);
    sqlite3_free(messaggeError);
    request = "INSERT INTO stock_market_data (id, fromm, too, price, "
              "payment_method, lower_limit, upper_limit, available, rating)"
              "VALUES (4, 'rub', 'usdt', -1, '', -1, -1, -1, -1);";
    exitt = sqlite3_exec(stock_market_data, request.c_str(), NULL, 0,
                         &messaggeError);
    sqlite3_free(messaggeError);
    request = "INSERT INTO stock_market_data (id, fromm, too, price, "
              "payment_method, lower_limit, upper_limit, available, rating)"
              "VALUES (5, 'rub', 'usdt', -1, '', -1, -1, -1, -1);";
    exitt = sqlite3_exec(stock_market_data, request.c_str(), NULL, 0,
                         &messaggeError);
    sqlite3_free(messaggeError);
    while (true) {
      func();
      std::cout << "update\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    }
  }).detach();
}

const std::string from = "RUB", to = "USDT";
unsigned int random_int;
double price = 88;
const std::vector<std::string> payment_methods = {"Raiffeisenbank", "A-Bank",
                                                  "CitiBank (Russia)"};
std::string payment_method;
double lower_limit = 500, upper_limit = 3500000;
double available = 50000;
int rating = 4;
const std::string link;

void one_iteration() {
  std::string request;
  for (int i = 1; i <= 5; ++i) {
    _rdrand32_step(&random_int);
    price += (double)(random_int % 101) / 100 * 4;
    payment_method = payment_methods[random_int % 3];
    available -= random_int % 49990 + (double)(random_int % 10001) / 10000;
    while (upper_limit > available * price) {
      upper_limit -= available * price;
    }
    upper_limit = std::floor(upper_limit * 100) / 100;
    if (upper_limit < 500) {
      upper_limit = 500;
    }
    if (upper_limit == 500) {
      lower_limit = 500;
    } else {
      lower_limit += random_int % (int)(upper_limit - 500) +
                     (double)(random_int % 100) / 100;
    }
    rating += (random_int % 7);
    request = "UPDATE stock_market_data SET"
              " price = " +
              std::to_string(price) +
              ", "
              "payment_method = '" +
              payment_method +
              "', "
              "lower_limit = " +
              std::to_string(lower_limit) +
              ", "
              "upper_limit = " +
              std::to_string(upper_limit) +
              ", "
              "available = " +
              std::to_string(available) +
              ", "
              "rating = " +
              std::to_string(rating) +
              " "
              "WHERE id = " +
              std::to_string(i);
    exitt = sqlite3_exec(stock_market_data, request.c_str(), NULL, 0,
                         &messaggeError);
    sqlite3_free(messaggeError);
    price = 88;
    lower_limit = 500;
    upper_limit = 3500000;
    available = 50000;
    rating = 4;
  }
}

int main() {
  order_generater(one_iteration, 10000);
  while (true) {
  }
}