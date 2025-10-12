#include <iostream>
#include "Logs.h"
#include "Container.h"

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

    LOG_INFO(PROTOCOLS::NONE, "Program Started");
    LOG_DEBUG(PROTOCOLS::NONE, "DEBUG MODE IS ENABLED");

    auto* settings = Container::instance().get();
#ifdef _WIN32
    SetConsoleTitleA(settings->program_name.c_str());
#else
    LOG_INFO(PROTOCOLS::NONE, "{}", settings->program_name);
#endif
    LOG_INFO(PROTOCOLS::NONE, "host : {}", settings->host);
    LOG_INFO(PROTOCOLS::NONE, "http_port : {}", settings->http_port);
    LOG_INFO(PROTOCOLS::NONE, "websocket_port : {}", settings->websocket_port);
    LOG_INFO(PROTOCOLS::NONE, "version : {}", settings->version);
    std::cin.get();
    return 0;
}