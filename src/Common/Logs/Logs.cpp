#include "Logs.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>

Logger::Logger() : debug(false) {
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    auto now = std::chrono::system_clock::now();
    std::string date = std::format("{:%m_%d_%Y}", now);
    std::string info_file = "Server_" + date + ".log";
    std::string debug_file = "Debug_" + date + ".log";
    std::string errors_file = "Errors_" + date + ".log";
    boost::filesystem::path dir("Logs");

    if (!boost::filesystem::exists(dir))
        boost::filesystem::create_directory(dir);

    boost::filesystem::path path_info = (dir / info_file).string();
    boost::filesystem::path path_debug = (dir / debug_file).string();
    boost::filesystem::path path_errors = (dir / errors_file).string();

    // for Info Logging
    boost::log::add_file_log(
        boost::log::keywords::file_name = path_info,
        boost::log::keywords::format = file_custom,
        boost::log::keywords::auto_flush = true
    )->set_filter(boost::log::trivial::severity == boost::log::trivial::info);

    if (debug == true)
    {
        // For Debugging
        boost::log::add_file_log(
            boost::log::keywords::file_name = path_debug,
            boost::log::keywords::format = file_custom,
            boost::log::keywords::auto_flush = true
        )->set_filter(boost::log::trivial::severity <= boost::log::trivial::debug);
    }

    // For Errors
    boost::log::add_file_log(
        boost::log::keywords::file_name = path_errors,
        boost::log::keywords::format = file_custom,
        boost::log::keywords::auto_flush = true
    )->set_filter(boost::log::trivial::severity >= boost::log::trivial::warning);

    boost::log::add_console_log(
        std::clog,
        boost::log::keywords::format = custom,
        boost::log::keywords::auto_flush = true
    );

    boost::log::add_common_attributes();

    // std::cout
    static std::ofstream cout_file(path_info.string(), std::ios::app);
    static StreamBuffer buffer(std::cout.rdbuf(), cout_file.rdbuf());
    std::cout.rdbuf(&buffer);

    // std::cerr
    static std::ofstream cerr_file(path_errors.string(), std::ios::app);
    static StreamBuffer err_buffer(std::cerr.rdbuf(), cerr_file.rdbuf());
    std::cerr.rdbuf(&err_buffer);
}

Logger::~Logger() { }

void Logger::custom(const boost::log::record_view& record, boost::log::formatting_ostream& stream) {
    format(record, stream, true);
}

void Logger::file_custom(const boost::log::record_view& record, boost::log::formatting_ostream& stream) {
    format(record, stream, false);
}

void Logger::format(const boost::log::record_view& record, boost::log::formatting_ostream& stream, bool boolean) {
    const auto record_severity = record[boost::log::trivial::severity];
    const auto record_timestamp = boost::log::extract<boost::posix_time::ptime>("TimeStamp", record);
    auto record_protocol = boost::log::extract<Protocol>("Protocol", record);
    auto record_file = boost::log::extract<std::string>("File", record);
    auto record_line = boost::log::extract<int>("Line", record);
    auto record_func = boost::log::extract<std::string>("Function", record);
    auto record_thread = boost::log::extract<boost::log::attributes::current_thread_id::value_type>("ThreadID", record);

    static const std::locale locale(std::cout.getloc(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S"));
    std::stringstream ss;
    ss.imbue(locale);

    ss << (record_timestamp ? *record_timestamp : boost::posix_time::second_clock::local_time());

    const char* green = boolean ? COLOR_GREEN : "";
    const char* bright_yellow = boolean ? COLOR_BRIGHT_YELLOW : "";
    const char* reset = boolean ? COLOR_RESET : "";
    auto color = boolean ? get_severity_color(*record_severity) : "";
    auto severity = get_severity_name(*record_severity);
    auto protocol = get_protocol_name(*record_protocol);
    auto message = record[boost::log::expressions::smessage];

    stream << ss.str()
        << " [" << green << record_file << reset << ":" << record_line << "]"
        << " [" << bright_yellow << record_func << reset << "]"
        << " [" << record_thread << "]"
        << " [" << color << severity;

    if (record_protocol.get() != Protocol::None)
        stream << reset << ":" << color << protocol;

    stream << reset << "] : " << message << reset;
}
