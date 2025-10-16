#include "Server.h"
#include "Routes.h"
#include "Handle.h"
#include "Logs.h"
#include <memory>

Server::Server(short port) : port(port) {
    try
    {
        auto routes = std::make_shared<Routes>();
        auto http = Http(port, routes);
        Handle* handle(routes);
        handle->set()->Initialize();
        http.run();
    }
    catch(const std::exception& e)
    {
        LOG_FATAL(Protocol::Http, "{}", e.what());
    }
}