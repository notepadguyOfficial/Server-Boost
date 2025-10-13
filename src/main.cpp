#include <boost/version.hpp>
#include <pqxx/pqxx>
#include <iostream>
#include "Global.h"
#include "Logs.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[]) {
    bool debug_mode = false;

    for (int i = 0; i < argc; ++i) {
        if (std::string(argv[i]) == "--debug") {
            debug_mode = true;
            break;
        }
    }

    Logger::instance().set(debug_mode);
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
    auto* settings = Container::instance().get();
    std::cin.get();
    return EXIT_SUCCESS;
}