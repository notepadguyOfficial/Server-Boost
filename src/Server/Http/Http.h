#pragma once
#include "Context.h"
#include "Routes.h"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

#define GET (boost::beast::http::verb::get)
#define POST (boost::beast::http::verb::post)
#define PUT (boost::beast::http::verb::put)
#define PATCH (boost::beast::http::verb::patch)
#define DELETE (boost::beast::http::verb::delete_)

class Http {
private:
    short port;
    std::shared_ptr<Routes> routes;
    boost::asio::io_context ioc{1};
public:
    Http(short port, std::shared_ptr<Routes> routes) :
        port(port),
        routes(std::move(routes)) { }

    ~Http() = default;
    
    Http* get() {
        return this;
    }

    Http* session(boost::asio::ip::tcp::socket socket);
    Http* run();
    short Port() const;
};