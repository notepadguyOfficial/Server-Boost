#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Logs.h"

Logger::Logger() : debug(false) {
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    auto now = std::chrono::system_clock::now();
    std::string date = std::format("{:%m%d%Y}", now);
    std::string file = "Server_" + date + ".log";
    boost::filesystem::path dir("Logs");

    if (!boost::filesystem::exists(dir))
        boost::filesystem::create_directory(dir);

    boost::filesystem::path path = (dir / file).string();

    boost::log::add_file_log(
        boost::log::keywords::file_name = path,
        boost::log::keywords::format = file_custom,
        boost::log::keywords::auto_flush = true
    );

    boost::log::add_console_log(
        std::clog,
        boost::log::keywords::format = custom,
        boost::log::keywords::auto_flush = true
    );

    boost::log::add_common_attributes();
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
        << "[\033[32m" << boost::log::extract<std::string>("File", rec) << "\033[0m:" << boost::log::extract<int>("Line", rec) << "] "
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
        << boost::log::extract<std::string>("File", rec) << ":" << boost::log::extract<int>("Line", rec) << "] "
        << boost::log::extract<std::string>("Function", rec) << "] "
        << "[" << boost::log::extract<boost::log::attributes::current_thread_id::value_type>("ThreadID", rec) << "] "
        << "[" << SEVERITY_MAP.at(*record_severity);
    if (record_protocol.get() != PROTOCOLS::NONE)
        strm << ":" << PROTOCOL_MAP.at(record_protocol.get());
    strm
        << "] : "
        << rec[boost::log::expressions::smessage];
}