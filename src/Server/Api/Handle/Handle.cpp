#include "Handle.h"
#include "Http.h"

Handle* Handle::Initialize()
{
    routes->set()->Prefix("/v1");

    //Todo: add endpoints and bind to Controller

    // test
    routes->add(GET, "/test", [](Context* ctx) {
        ctx->Response(boost::beast::http::status::ok, "This is Test");
    });

    return this;
}