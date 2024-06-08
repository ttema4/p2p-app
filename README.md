# 🚀 P2P App - Description of the project

Our project is a **tool for analyzing cryptocurrency markets** and finding opportunities for profitable arbitrage. **The server** part **collects** and **analyzes** **data** from various Peer-to-Peer trading platforms, and **the application** **displays** **the results** of this analysis, opening **new** **prospects** for **investing** and **financial transactions** in the world of **cryptocurrencies**.

* **Server Technology Stack:** Boost::asio, NLohmann, Decimal, Magic Enum, moodycamel::ConcurrentQueue
* **Application Technology Stack:** Qt6, NLohmann, MySQL, Boost::asio.


> ***The application is being written, built and tested under Clang++17, macOS.***

## 🛠️ How to build?

``` shell
cmake -B build && cmake --build build && cmake --install build
```

## 🧩 Architecture

### 🔄 Crypto Exchange Parser

This microservice performs the following functions:

- Parsing data from crypto exchanges.
- Formatting data into a convenient **JSON** format.
- Sending data over **TCP**.

### 🔍 Arbitration Bundles search service

This microservice provides the following functions:

- Receiving data from the first microservice over **TCP**.
- Search for arbitrage bundles of cryptocurrencies.
- Interaction with clients via **TCP**.

### 🖥️ The client part

The client part of the application is responsible for:

- Receiving data over **TCP** from the server side.
- Data output to a table with support for filtering and sorting arbitration bundles.
- Implementation of an authorization system that allows users to add bundles to favorites.

The client part is designed using **Qt6**, providing powerful capabilities for building a graphical interface.

## 🧪 Testing
The following testing methods are implemented in the project:

* **Server part:**

  - Unit tests to check individual components of the system.
  - Logging for tracking execution and debugging.

* **The client part:**

  - Logging to track the execution and debugging of the client application.


## 🔚 Conclusion

Our project is a system designed to **analyze** the **P2P** market on **crypto exchanges**. It has **high speed**, **accuracy** and **reliability**, thanks to the use of **modern technologies** and **tools**. The entire analysis process becomes **intuitive** for the user.




## 📅 And the development area:
1. [First sketches of the UI](https://arc.net/e/482FD89B-F6DA-4F38-9423-1BED0E5B8C8F )
2. [Presentation with предзащиты](https://docs.google.com/presentation/d/1NrUOuDGumqUUWVuxRqMIdVQsbHfAztYD_oqa6h2195s/edit#slide=id.g2c944cb5879_4_62)
3. [Video recording of the pre-defense](https://youtu.be/aajr6Wu4m8k )