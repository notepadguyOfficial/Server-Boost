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
#include <regex>
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

/**
 * Capture and duplicate characters written to std::cout to both a primary (console) and a secondary (file) stream buffer,
 * preserving console formatting while writing a cleaned (ANSI escapes removed) copy to the secondary buffer.
 *
 * The instance buffers characters until a newline or explicit sync/destruction triggers a flush of the buffered line
 * to the secondary buffer with ANSI escape sequences removed.
 *
 * @param sb1 Pointer to the primary stream buffer (console) that receives the original characters.
 * @param sb2 Pointer to the secondary stream buffer (file) that receives a cleaned copy of buffered lines.
 *
 * The destructor flushes any remaining buffered content to the secondary buffer.
 */
class StreamBuffer : public std::streambuf {
public:
    StreamBuffer(std::streambuf* sb1, std::streambuf* sb2)
        : sb1_(sb1), sb2_(sb2) { }

    ~StreamBuffer() {
        flush();
    }

protected:
    int overflow(int c) override {
        if (c == EOF)
            return !EOF;
        if (sb1_->sputc(c) == EOF)
            return EOF;
        buffer_.push_back(static_cast<char>(c));
        if(c == '\n')
            flush();
        return c;
    }

    /**
     * Flushes internal buffer and synchronizes the primary stream buffer.
     *
     * Calls flush() to process any buffered characters, then invokes pubsync() on
     * the primary underlying stream buffer. Returns 0 if the primary buffer's
     * pubsync() reports success, -1 otherwise.
     *
     * @returns 0 on success, -1 on failure.
     */
    int sync() override {
        flush();
        return (sb1_->pubsync() == 0) ? 0 : -1;
    }

private:
    std::streambuf* sb1_; // console
    std::streambuf* sb2_; // file
    std::string buffer_;

    /**
     * Flush buffered characters to the secondary stream after removing ANSI escape sequences.
     *
     * If there is buffered data, this function removes ANSI color/formatting escape sequences,
     * writes the cleaned data to the secondary stream buffer, synchronizes that stream, and
     * clears the internal buffer. If the buffer is empty, no action is taken.
     */
    void flush() {
        if (buffer_.empty())
            return;
        static const std::regex ansi_regex(R"(\x1B\[[0-9;]*[A-Za-z])");
        std::string clean = std::regex_replace(buffer_, ansi_regex, "");
        sb2_->sputn(clean.data(), clean.size());
        sb2_->pubsync();
        buffer_.clear();
    }
};

/**
     * Get the process-wide Logger singleton instance.
     *
     * @returns Reference to the single, shared Logger instance. 
     */
    class Logger {
public:
    static Logger& instance() {
        static Logger instance;
        return instance;
    }

    boost::log::sources::severity_logger<boost::log::trivial::severity_level>& get() {
        return logger;
    }

    /**
     * Toggle debug logging and update the global log filter accordingly.
     *
     * When enabled, the global Boost.Log filter is set to allow messages with
     * severity debug or higher; when disabled, it is set to allow messages with
     * severity info or higher.
     *
     * @param boolean `true` to enable debug-level logging, `false` to restrict logging to info and above.
     */
    void set(bool boolean) {
        debug = boolean;
        if (debug == true)
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