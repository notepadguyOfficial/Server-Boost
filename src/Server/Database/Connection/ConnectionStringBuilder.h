#pragma once
#include "Global.h"
#include <string>
#include <sstream>
#include <pqxx/pqxx>
#include <cctype>

class ConnectionStringBuilder {
public:
    std::string build() const {
        auto* options = settings();

        std::string str = build_string(options);

        if (test(str))
            return str;

        if (!options->db_name.empty()) {
            char temporary = options->db_name[0];
            options->db_name[0] = std::toupper(static_cast<unsigned char>(options->db_name[0]));

            str = build_string(options);

            if(test(str)) {
                return str;
            }

            options->db_name[0] = temporary;
        }
        return str;
    }

private:
    std::string build_string(const Settings* options) const {
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

    bool test(const std::string& temp) const {
        try {
            pqxx::connection connect(temp);
            return connect.is_open();
        }
        catch(const std::exception& e)
        {
            return false; 
        }  
    }
};
