#ifndef JSON_TOOLS_HPP_
#define JSON_TOOLS_HPP_

#include <string>

#include "include/nlohmann/json.hpp"
#include "storage_structures.hpp"

namespace p2p {

void fix_banks_naming_and_filter(Orders &orders);

void unpack_json(std::string parsers_response, Orders &orders,
                 MarketRates &market_rates);

std::string pack_json(Chains &chains);

}  // namespace p2p

#endif  // JSON_TOOLS_HPP_
