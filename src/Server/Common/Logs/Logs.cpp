#include "Logs.h"

Logger::Logger(const std::string& file) {
    boost::log::add_file_log(
        boost::log::keywords::file_name = file,
        boost::log::keywords::auto_flush = true
    );

    boost::log::add_console_log(
        std::clog,
        boost::log::keywords::format = "%TimeStamp% [%File%:%%LineID%%] [%Function%] [%ThreadID%:%ProcessID%] [%Severity%] %Message%",
        boost::log::keywords::auto_flush = true
    );

#ifdef DEBUGGING
    boost::log::core::get()->set_filter     (        
        boost::log::trivial::severity >= boost::log::trivial::debug     
    );
#else
    boost::log::core::get()->set_filter     (        
        boost::log::trivial::severity >= boost::log::trivial::info     
    );
#endif
    boost::log::core::get()->add_global_attribute("File", boost::log::attributes::constant<std::string>(__FILE__));
    boost::log::add_common_attributes();
}

Logger::~Logger() { }

void Logger::log_msg(const std::string& message, const boost::log::trivial::severity_level severity, const PROTOCOLS protocol) {
    boost::lock_guard<boost::mutex> guard(mutex_);
    switch (protocol) {
    case PROTOCOLS::NONE:
        BOOST_LOG_SEV(logger, severity) << ": " << message;
        break;
    case PROTOCOLS::HTTP:
        BOOST_LOG_SEV(logger, severity) << "[HTTP]: " << message;
        break;
    case PROTOCOLS::WEBSOCKET:
        BOOST_LOG_SEV(logger, severity) << "[WEBSOCKET]: " << message;
        break;
    case PROTOCOLS::DATABASE:
        BOOST_LOG_SEV(logger, severity) << "[DATABASE]: " << message;
        break;
    }
}
