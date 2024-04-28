#ifndef DATA_PROCESSING_HPP_
#define DATA_PROCESSING_HPP_

#include "storage_structures.hpp"

namespace p2p {

// struct DataReceiver {
//   void receive() {}
// };

struct Analysis {
  void analyze(Chains &chains, Orders &orders_for_buy, Orders &orders_for_sell,MarketRates &market_rates);
};

// struct DataSender {
//   void send() {}
// };

} // namespace p2p

#endif // DATA_PROCESSING_HPP_
