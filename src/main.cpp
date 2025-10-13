#include <boost/version.hpp>
#include <pqxx/pqxx>
#include <iostream>
#include "Global.h"
#include "Logs.h"
#include "Connection.h"
#include "QueryBuilder.h"
#include "Helper.h"

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * @brief Program entry point that initializes logging, displays startup banners, verifies PostgreSQL compatibility, and runs startup checks.
 *
 * Initializes runtime mode from command-line arguments (recognizes "--debug"), configures the global logger, prints application/version banners and library versions, verifies the connected PostgreSQL server meets the minimum required version, and waits for user input before exiting.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument vector; if it contains "--debug" the program enables debug mode.
 * @return int `EXIT_SUCCESS` on normal completion, `EXIT_FAILURE` if the PostgreSQL server version is below the minimum required version.
 */
int main(int argc, char* argv[]) {
    bool debug_mode = false;

    for (int i = 0; i < argc; ++i) {
        if (std::string(argv[i]) == "--debug") {
            debug_mode = true;
            break;
        }
    }

    Logger::instance().set(debug_mode);
    auto* settings = Container::instance().get();
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitleA(APP_NAME);
#endif
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
#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, 3);
#else
    std::cout << "\033[36m";
#endif
    std::cout << R"(
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
#ifdef _WIN32
    SetConsoleTextAttribute(hConsole, 7);
#else
    std::cout << "\033[0m" << std::endl;
#endif
    if (debug_mode == true)
        std::cout << "DEBUG MODE IS ENABLED" << std::endl;

    // Testing
    // Spent 7 hours to debug and fix this shit
    // Found out before going to sleep that the reason why this shit didnt work was because I was using the old Documentation that is for Version 6.9
    auto connection = Connection::instance().acquire();
    QueryBuilder builder(*connection); // i forget to pass connection to this mf
    pqxx::work txn(*connection);

    // hardcoded shits
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
        std::cout << std::endl;

        std::cout << SYMBOL_WRONG_PRINT << "Please use atleast Postgresql ";
        print_version(MINIMUM_REQUIRED_POSTGRESQL_VERSION);
        std::cout << " or above." << std::endl;

        return EXIT_FAILURE;
    }
    else
    {
        std::cout << SYMBOL_CHECK_PRINT << "Postgresql Server: ";
        print_version(POSTGRESQL_VERSION);
        std::cout << std::endl;
    }

    std::cin.get();
    return EXIT_SUCCESS;
}