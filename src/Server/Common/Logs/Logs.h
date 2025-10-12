#pragma once
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <format>
#include <fstream>
#include <sstream>
#include <vector>

enum PROTOCOLS {
    NONE,
    HTTP,
    WEBSOCKET,
    DATABASE,
    SETTINGS
};

const std::map<PROTOCOLS, std::string> PROTOCOL_MAP = {
    {PROTOCOLS::NONE, "NONE"},
    {PROTOCOLS::HTTP, "HTTP"},
    {PROTOCOLS::WEBSOCKET, "WEBSOCKET"},
    {PROTOCOLS::DATABASE, "DATABASE"},
    {PROTOCOLS::SETTINGS, "SETTINGS"}
};

const std::map<boost::log::trivial::severity_level, std::string> SEVERITY_MAP = {
    {boost::log::trivial::trace, "TRACE"},
    {boost::log::trivial::debug, "DEBUG"},
    {boost::log::trivial::info, "INFO"},
    {boost::log::trivial::warning, "WARNING"},
    {boost::log::trivial::error, "ERROR"},
    {boost::log::trivial::fatal, "FATAL"}
};

const std::map<boost::log::trivial::severity_level, std::string> SEVERITY_COLOR_MAP = {
    {boost::log::trivial::trace, "\033[90m"},
    {boost::log::trivial::debug, "\033[34m"},
    {boost::log::trivial::info, "\033[32m"},
    {boost::log::trivial::warning, "\033[33m"},
    {boost::log::trivial::error, "\033[31m"},
    {boost::log::trivial::fatal, "\033[1;31;40m"}
};

class Logger {
public:
    static Logger& instance() {
        static Logger instance;
        return instance;
    }
    boost::log::sources::severity_logger<boost::log::trivial::severity_level>& get() {
        return logger;
    }
    void set(bool boolean) {
        debug = boolean;
        if (debug)
            boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
        else
            boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
    }
private:
    Logger();
    ~Logger();
    static void custom(const boost::log::record_view& rec, boost::log::formatting_ostream& strm);
    static void file_custom(const boost::log::record_view& rec, boost::log::formatting_ostream& strm);
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> logger;
    boost::mutex mutex_;
    bool debug;
};

// Helper
inline std::string trim(const std::string& path) {
    const std::string base = "src";
    boost::filesystem::path p(path);
    std::string temp = p.generic_string();

    auto position = temp.find(base + "/");
    if(position != std::string::npos)
        return temp.substr(position);
    return p.filename().string();
}

#define WRITE_TO_LOG(__SEVERITY__, __PROTOCOL__, __FMT__, ...) \
    BOOST_LOG_SEV(Logger::instance().get(), __SEVERITY__) \
        << boost::log::add_value("File", trim(__FILE__)) \
        << boost::log::add_value("Line", __LINE__) \
        << boost::log::add_value("Function", __func__) \
        << boost::log::add_value("Protocol", __PROTOCOL__) \
        << std::format(__FMT__, ##__VA_ARGS__)

#define LOG_TRACE(__PROTOCOL__, fmt, ...) \
    WRITE_TO_LOG(boost::log::trivial::trace, __PROTOCOL__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(__PROTOCOL__, fmt, ...) \
    WRITE_TO_LOG(boost::log::trivial::debug, __PROTOCOL__, fmt, ##__VA_ARGS__)
#define LOG_INFO(__PROTOCOL__, fmt, ...) \
    WRITE_TO_LOG(boost::log::trivial::info, __PROTOCOL__, fmt, ##__VA_ARGS__)
#define LOG_WARNING(__PROTOCOL__, fmt, ...) \
    WRITE_TO_LOG(boost::log::trivial::warning, __PROTOCOL__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(__PROTOCOL__, fmt, ...) \
    WRITE_TO_LOG(boost::log::trivial::error, __PROTOCOL__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(__PROTOCOL__, fmt, ...) \
    WRITE_TO_LOG(boost::log::trivial::fatal, __PROTOCOL__, fmt, ##__VA_ARGS__)

extern Logger g_logger;
