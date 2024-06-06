#include "globalcondition.h"

GlobalCondition &GlobalCondition::getInstance() {
    static GlobalCondition instance;
    return instance;
}

void GlobalCondition::setFromJson(const nlohmann::json &json) {
    bool allRight = true;

    if (json.contains("use_database")) {
        use_database = json["use_database"];
    } else {
        allRight = false;
    }
    if (json.contains("use_server")) {
        use_server = json["use_server"];
    } else {
        allRight = false;
    }
    if (json.contains("server_ip")) {
        server_ip = json["server_ip"];
    } else {
        allRight = false;
    }
    if (json.contains("server_port")) {
        server_port = json["server_port"];
    } else {
        allRight = false;
    }
    if (json.contains("server_update_interval")) {
        server_update_interval = json["server_update_interval"];
    } else {
        allRight = false;
    }
    if (json.contains("database_ip")) {
        database_ip = json["database_ip"];
    } else {
        allRight = false;
    }
    if (json.contains("database_port")) {
        database_port = json["database_port"];
    } else {
        allRight = false;
    }
    if (json.contains("database_dataname")) {
        database_dataname = json["database_dataname"];
    } else {
        allRight = false;
    }
    if (json.contains("database_username")) {
        database_username = json["database_username"];
    } else {
        allRight = false;
    }
    if (json.contains("database_userpass")) {
        database_userpass = json["database_userpass"];
    } else {
        allRight = false;
    }

    if (!allRight) {
        throw QException();
    }
}
