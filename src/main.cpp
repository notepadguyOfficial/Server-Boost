#include "Console.h"
#include <iostream>

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
    return EXIT_SUCCESS;
}