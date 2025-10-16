#include "Console.h"
#include "Container.h"
#include "Logs.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
    bool debug_mode = false;

    for (int i = 0; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--debug")
        {
            debug_mode = true;
            break;
        }
    }

    Logger::instance().set(debug_mode);
    auto *settings = Container::instance().get();
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitleA(APP_NAME);
#endif
    Console console(debug_mode);
    console.Show();
    std::cin.get();
    return EXIT_SUCCESS;
}