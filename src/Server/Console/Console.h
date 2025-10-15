#pragma once
#include <boost/version.hpp>
#include <pqxx/pqxx>
#include "Global.h"
#include "Logs.h"
#include "Connection.h"
#include "QueryBuilder.h"
#include "Helper.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

class Console
{
    bool debug;

public:
    explicit Console(bool debug) : debug(debug) {}
    void Show();
};
