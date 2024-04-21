#ifndef DATARECIEVER_H
#define DATARECIEVER_H

#include "src/nlohmann/json.hpp"
#include "logic_fwd.hpp"
#include "logic_simulator.h"
#include <QVector>

struct DataReciever {
    // какой-то ip, какой-то сервер
    DataReciever() = default;

    QVector<Chain> recieveNewChain() {
        QVector<Chain> chains = nlohmann::json::parse(getJsonToString()).get<QVector<Chain>>();
        return chains;
    }
};


#endif // DATARECIEVER_H
