#ifndef DATA_PROCESSING_HPP_
#define DATA_PROCESSING_HPP_

#include "storage_structures.hpp"

namespace p2p {

struct Analysis {
    void analyze(Chains &chains, Orders &orders, MarketRates &market_rates);
};

}  // namespace p2p

#endif  // DATA_PROCESSING_HPP_
