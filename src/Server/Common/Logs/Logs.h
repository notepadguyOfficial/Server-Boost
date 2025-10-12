#pragma once
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/thread.hpp>
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
    DATABASE
};

class Logger {
public:
    static Logger& instance() {
        auto now = std::chrono::system_clock::now();
        std::string date = std::format("{:%m%d%Y}", now);
        std::string file = "Server_" + date + ".log";
        static Logger logger(file);
        return logger;
    }

    template <typename... Args>
    void log_msg(boost::log::trivial::severity_level severity, PROTOCOLS protocol, const std::string& fmt, Args&&... args) {
        std::string message = std::format(fmt, std::forward<Args>(args)...);
        log_msg(message, severity, protocol);
    }

    void log_msg(boost::log::trivial::severity_level severity, PROTOCOLS protocol, const std::string& fmt) {
        log_msg(fmt, severity, protocol);
    }

private:
    Logger(const std::string& file);
    ~Logger();
    void log_msg(const std::string& message, const boost::log::trivial::severity_level severity, const PROTOCOLS protocol);
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> logger;
    boost::mutex mutex_;
};

#define LOG_TRACE(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::trace, NONE, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::debug, NONE, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::info, NONE, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::warning, NONE, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::error, NONE, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::fatal, NONE, fmt, ##__VA_ARGS__)

// Http Protocol Logging
#define LOG_HTTP_TRACE(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::trace, HTTP, fmt, ##__VA_ARGS__)
#define LOG_HTTP_DEBUG(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::debug, HTTP, fmt, ##__VA_ARGS__)
#define LOG_HTTP_INFO(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::info, HTTP, fmt, ##__VA_ARGS__)
#define LOG_HTTP_WARNING(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::warning, HTTP, fmt, ##__VA_ARGS__)
#define LOG_HTTP_ERROR(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::error, HTTP, fmt, ##__VA_ARGS__)
#define LOG_HTTP_FATAL(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::fatal, HTTP, fmt, ##__VA_ARGS__)

// Websocket Protocol Logging
#define LOG_WEBSOCKET_TRACE(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::trace, WEBSOCKET, fmt, ##__VA_ARGS__)
#define LOG_WEBSOCKET_DEBUG(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::debug, WEBSOCKET, fmt, ##__VA_ARGS__)
#define LOG_WEBSOCKET_INFO(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::info, WEBSOCKET, fmt, ##__VA_ARGS__)
#define LOG_WEBSOCKET_WARNING(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::warning, WEBSOCKET, fmt, ##__VA_ARGS__)
#define LOG_WEBSOCKET_ERROR(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::error, WEBSOCKET, fmt, ##__VA_ARGS__)
#define LOG_WEBSOCKET_FATAL(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::fatal, WEBSOCKET, fmt, ##__VA_ARGS__)

// Database Protocol Logging
#define LOG_DATABASE_TRACE(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::trace, DATABASE, fmt, ##__VA_ARGS__)
#define LOG_DATABASE_DEBUG(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::debug, DATABASE, fmt, ##__VA_ARGS__)
#define LOG_DATABASE_INFO(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::info, DATABASE, fmt, ##__VA_ARGS__)
#define LOG_DATABASE_WARNING(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::warning, DATABASE, fmt, ##__VA_ARGS__)
#define LOG_DATABASE_ERROR(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::error, DATABASE, fmt, ##__VA_ARGS__)
#define LOG_DATABASE_FATAL(fmt, ...) \
    Logger::instance().log_msg(boost::log::trivial::fatal, DATABASE, fmt, ##__VA_ARGS__)
