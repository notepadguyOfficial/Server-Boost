#pragma once
#include "Container.h"
#include "Version.h"

#define MINIMUM_POSTGRESQL_VERSION "17.0"

/*
 *
 *   Usage e.g
 *       auto* options = settings();
 *       std::cout << "Host: " << options->host << std::endl;
 *
 */
inline Settings *settings()
{
    return Container::instance().get();
}
