#include "Console.h"
#include "Server.h"
#include "Global.h"
#include "Logs.h"

#include <iostream>
#include <csignal>
#include <atomic>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

#include <boost/version.hpp>
#include <pqxx/pqxx>
#include "Connection.h"
#include "QueryBuilder.h"
#include "Helper.h"

void Console::Show()
{
    auto *options = settings();

    std::cout
        << APP_NAME
        << " rev. "
        << GIT_COMMIT_HASH
        << " "
        << GIT_COMMIT_DATE
        << " ("
        << GIT_BRANCH
        << " branch) "
        << "("
        << ARCH_NAME
        << ")"
        << std::endl;
    std::cout << "<Ctrl + C> to stop." << std::endl;

    std::cout << COLOR_CYAN << R"(
        ██╗░░██╗██╗░░░██╗██████╗░██████╗░░█████╗░
        ██║░░██║╚██╗░██╔╝██╔══██╗██╔══██╗██╔══██╗
        ███████║░╚████╔╝░██║░░██║██████╔╝██║░░██║
        ██╔══██║░░╚██╔╝░░██║░░██║██╔══██╗██║░░██║
        ██║░░██║░░░██║░░░██████╔╝██║░░██║╚█████╔╝
        ╚═╝░░╚═╝░░░╚═╝░░░╚═════╝░╚═╝░░╚═╝░╚════╝░
                            ██╗░░██╗██╗░░░██╗██████╗░
                            ██║░░██║██║░░░██║██╔══██╗
                            ███████║██║░░░██║██████╦╝
                            ██╔══██║██║░░░██║██╔══██╗
                            ██║░░██║╚██████╔╝██████╦╝
                            ╚═╝░░╚═╝░╚═════╝░╚═════╝░
    )" << std::endl;
    std::cout
        << "\t"
        << GIT_REPO_NAME
        << " "
        << APP_VERSION
        << " - "
        << GIT_REPO_URL
        << std::endl;

    std::cout << COLOR_RESET << std::endl;

    if (debug == true)
        std::cout << "DEBUG MODE IS ENABLED" << std::endl;

    auto connection = Connection::instance().acquire();
    QueryBuilder builder(*connection);
    pqxx::work txn(*connection);

    pqxx::result result = txn.exec("SHOW server_version;");
    auto POSTGRESQL_VERSION = parse(std::string(result[0][0].c_str()));
    auto MINIMUM_REQUIRED_POSTGRESQL_VERSION = parse(MINIMUM_POSTGRESQL_VERSION);
    Connection::instance().release(connection);

    std::cout
        << SYMBOL_CHECK_PRINT
        << "Boost Library: "
        << BOOST_VERSION / 100000
        << "."
        << BOOST_VERSION / 100 % 1000
        << "."
        << BOOST_VERSION % 100
        << std::endl;

    std::cout << SYMBOL_CHECK_PRINT << "libpqxx Library: " << PQXX_VERSION << std::endl;

    if (check_version(POSTGRESQL_VERSION, MINIMUM_REQUIRED_POSTGRESQL_VERSION))
    {
        std::cout << SYMBOL_WRONG_PRINT << "Postgresql Server: ";
        print_version(POSTGRESQL_VERSION);

        std::cout << " → Please use atleast Postgresql ";
        print_version(MINIMUM_REQUIRED_POSTGRESQL_VERSION);
        std::cout << " or above!" << std::endl;

        return exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << SYMBOL_CHECK_PRINT << "Postgresql Server: ";
        print_version(POSTGRESQL_VERSION);
        std::cout << std::endl;
    }

    short port = static_cast<short>(options->http_port);

    try
    {
        Server server(port);
    }
    catch (const std::exception& e)
    {
        LOG_FATAL(Protocol::Http, "{}", e.what());
        exit(EXIT_FAILURE);
    }
    
}
