#pragma once
#include "Context.h"
#include "Routes.h"
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <map>
#include <functional>
#include <string>

class Handle {
private:
    std::shared_ptr<Routes> routes;

public:
    Handle(std::shared_ptr<Routes> routes) : routes(std::move(routes)) { }
    ~Handle() { }
    const Handle* get() const {
        return this;
    }

    Handle* set() {
        return this;
    }

    Handle* Initialize();
};