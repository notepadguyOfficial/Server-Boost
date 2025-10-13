#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
#include <string>

struct Settings {
    std::string host;
    uint16_t http_port;
    uint16_t websocket_port;
    std::string db_host;
    uint16_t db_port;
    std::string db_name;
    std::string db_user;
    std::string db_pass;
    std::string db_mode;
};

/**
     * Access the singleton Container instance.
     * @returns Reference to the global Container singleton.
     */
    class Container {
public:
    static Container& instance() {
        static Container instance;
        return instance;
    }
    Settings* get() const {
        return settings;
    }
private:
    Container();
    ~Container();
    void load();
    void create();
    // void write(); - ToDo
private:
    Settings* settings = nullptr;
    const std::string config = "Settings.json"; 
};