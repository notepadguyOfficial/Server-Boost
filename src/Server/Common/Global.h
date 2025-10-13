#pragma once
#include "Version.h"
#include "Container.h"

#define SYMBOL_CHECK_PRINT "[\033[32m✓\033[0m]"
#define SYMBOL_WRONG_PRINT "[\033[31mX\033[0m]"

/*
*
*   Usage e.g
*       auto* options = settings();
*       std::cout << "Host: " << options->host << std::endl;
*
*/
inline Settings* settings() {
    return Container::instance().get();
}
