#pragma once
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <chrono>
#include <format>
#include "TextExtra.h"
#include "StreamBuffer.h"

class Logger
{
public:
    static Logger &instance()
    {
        static Logger instance;
        return instance;
    }

    boost::log::sources::severity_logger<boost::log::trivial::severity_level> &get()
    {
        return logger;
    }

    void set(bool boolean)
    {
        debug = boolean;
        if (debug == true)
            boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
        else
            boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
    }

private:
    Logger();
    ~Logger();
    static void custom(const boost::log::record_view &record, boost::log::formatting_ostream &stream);
    static void file_custom(const boost::log::record_view &record, boost::log::formatting_ostream &stream);
    static void format(const boost::log::record_view &record, boost::log::formatting_ostream &stream, bool boolean);
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> logger;
    boost::mutex mutex_;
    bool debug;
};

#define WRITE_TO_LOG(__SEVERITY__, __PROTOCOL__, __FMT__, ...) \
    BOOST_LOG_SEV(Logger::instance().get(), __SEVERITY__)      \
        << boost::log::add_value("File", trim(__FILE__))       \
        << boost::log::add_value("Line", __LINE__)             \
        << boost::log::add_value("Function", __func__)         \
        << boost::log::add_value("Protocol", __PROTOCOL__)     \
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
