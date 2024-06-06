#ifndef GLOBALCONDITION_H
#define GLOBALCONDITION_H

#include <string>
#include <nlohmann/json.hpp>
#include <QException>

struct GlobalCondition {
    static GlobalCondition &getInstance();
    void setFromJson(const nlohmann::json& json);

    bool use_database;
    bool use_server;

    std::string server_ip;
    short server_port;

    short server_update_interval;

    std::string database_ip;
    short database_port;

    std::string database_dataname;
    std::string database_username;
    std::string database_userpass;

    bool serverOk = true;
    bool databaseOk = true;
};

#endif  // GLOBALCONDITION_H
