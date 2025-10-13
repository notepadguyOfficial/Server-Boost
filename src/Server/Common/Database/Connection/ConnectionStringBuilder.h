#pragma once
#include "Global.h"
#include <string>
#include <sstream>

class ConnectionStringBuilder {
public:
    std::string build() const {
        auto* options = settings();

        std::ostringstream oss;
        oss << "host=" << options->host << " ";
        oss << "port=" << options->db_port << " ";
        oss << "dbname=" << options->db_name << " ";
        oss << "user=" << options->db_user << " ";
        oss << "password=" << options->db_pass << " ";
        oss << "sslmode=" << options->db_mode << " ";

        std::string result = oss.str();
        if (!result.empty() && result.back() == ' ')
            result.pop_back();

        return result;
    }
};
