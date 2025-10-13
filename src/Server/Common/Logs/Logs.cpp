#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Logs.h"

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

void Logger::custom(const boost::log::record_view& rec, boost::log::formatting_ostream& strm) {
    const auto record_severity = rec[boost::log::trivial::severity];
    const auto record_timestamp = boost::log::extract<boost::posix_time::ptime>("TimeStamp", rec);
    const std::locale loc(std::cout.getloc(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S"));
    auto record_protocol = boost::log::extract<PROTOCOLS>("Protocol", rec);
    std::stringstream ss;

    ss.imbue(loc);

    ss << record_timestamp;
    strm
        << ss.str()
        << " [\033[32m" << boost::log::extract<std::string>("File", rec) << "\033[0m:" << boost::log::extract<int>("Line", rec) << "] "
        << "[\033[93m" << boost::log::extract<std::string>("Function", rec) << "\033[0m] "
        << "[" << boost::log::extract<boost::log::attributes::current_thread_id::value_type>("ThreadID", rec) << "] "
        << "[" << SEVERITY_COLOR_MAP.at(*record_severity)
        << SEVERITY_MAP.at(*record_severity);
    if (record_protocol.get() != PROTOCOLS::NONE) {
        strm
            << "\033[0m:" << SEVERITY_COLOR_MAP.at(*record_severity)
            << PROTOCOL_MAP.at(record_protocol.get());
    }
    strm
        << "\033[0m] : "
        << SEVERITY_COLOR_MAP.at(*record_severity)
        << rec[boost::log::expressions::smessage]
        << "\033[0m";
}

void Logger::file_custom(const boost::log::record_view& rec, boost::log::formatting_ostream& strm) {
    const auto record_severity = rec[boost::log::trivial::severity];
    const auto record_timestamp = boost::log::extract<boost::posix_time::ptime>("TimeStamp", rec);
    const std::locale loc(std::cout.getloc(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S"));
    auto record_protocol = boost::log::extract<PROTOCOLS>("Protocol", rec);
    std::stringstream ss;

    ss.imbue(loc);

    ss << record_timestamp;
    strm
        << ss.str()
        << " [" << boost::log::extract<std::string>("File", rec) << ":" << boost::log::extract<int>("Line", rec) << "] "
        << "[" << boost::log::extract<std::string>("Function", rec) << "] "
        << "[" << boost::log::extract<boost::log::attributes::current_thread_id::value_type>("ThreadID", rec) << "] "
        << "[" << SEVERITY_MAP.at(*record_severity);
    if (record_protocol.get() != PROTOCOLS::NONE)
        strm << ":" << PROTOCOL_MAP.at(record_protocol.get());
    strm
        << "] : "
        << rec[boost::log::expressions::smessage];
}