#include "Container.h"
#include "Logs.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

Container::Container() {
    settings = new Settings();
    load();
}

Container::~Container() { 
    delete settings;
}

void Container::load() {
    boost::property_tree::ptree tree;

    std::ifstream file(config);

    if (!file.good()) {
        LOG_DEBUG(Protocol::Settings, "Config file not found. Creating default configuration.");
        create();

        return;
    }

    try {
        boost::property_tree::read_json(config, tree);
        settings->host = tree.get<std::string>("host", "0.0.0.0");
        settings->http_port = tree.get<uint16_t>("http_port", 8080);
        settings->websocket_port = tree.get<uint16_t>("websocket_port", 9090);
        settings->db_host = tree.get<std::string>("db_host", "localhost");
        settings->db_port = tree.get<uint16_t>("db_port", 5432);
        settings->db_name = tree.get<std::string>("db_name", "capstone");
        settings->db_user = tree.get<std::string>("db_user", "postgres");
        settings->db_pass = tree.get<std::string>("db_pass", "postgres");
        settings->db_mode = tree.get<std::string>("db_mode", "disable");

        LOG_DEBUG(Protocol::Settings, "Configuration loaded from {}.", config);
    }
    catch(std::exception& e) {
        LOG_FATAL(Protocol::Settings, "Error loading config [{}]", e.what());
        create();
    }
}

void Container::create() {
    boost::property_tree::ptree tree;

    settings->host = "0.0.0.0";
    settings->http_port = 8080;
    settings->websocket_port = 9090;
    settings->db_host = "localhost";
    settings->db_port = 5432;
    settings->db_name = "capstone";
    settings->db_user = "postgres";
    settings->db_pass  = "postgres";
    settings->db_mode = "disable";

    tree.put("host", settings->host);
    tree.put("http_port", settings->http_port);
    tree.put("websocket_port", settings->websocket_port);
    tree.put("db_host", settings->db_host);
    tree.put("db_port", settings->db_port);
    tree.put("db_user", settings->db_user);
    tree.put("db_pass", settings->db_pass);
    tree.put("db_name", settings->db_name);
    tree.put("db_mode", settings->db_mode);
    try {
        boost::property_tree::write_json(config, tree);
        LOG_DEBUG(Protocol::Settings, "Default configuration file created {}.", config);
    }
    catch (std::exception& e) {
        LOG_FATAL(Protocol::Settings, "Error writing default config [{}]", e.what());
    }
}