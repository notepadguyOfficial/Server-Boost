#include <iostream>
#include "Server/Common/Logs/Logs.h"

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; ++i) {
        if (std::string(argv[i]) == "--debug") {
            #define DEBUGGING
            break;
        }
    }
    
    LOG_INFO("Program is Starting...");
    return 0;
}