#pragma once
#include "context.h"
#include <boost/beast.hpp>
#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <string>

class Routes {
private:
    struct Info {
        boost::beast::http::verb method;
        std::string pattern;
        std::function<void(Context*)> handler;
    };

    std::vector<Info> routes;
    std::string prefix;
    std::vector<std::string> split(const std::string &path);

public:
    Routes* get() {
        return this;
    }

    Routes* set() {
        return this;
    }

    Routes* add(
        boost::beast::http::verb method,
        const std::string &pattern,
        std::function<void(Context*)> handler
    );

    bool route(Context* ctx);
    Routes* Prefix(const std::string &p);
};