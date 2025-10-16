#include "Context.h"

const boost::beast::http::request<boost::beast::http::string_body>& Context::Request() const
{
    return request;
}

boost::beast::http::response<boost::beast::http::string_body>& Context::Response()
{
    return response;
}

std::string Context::Parameter(const std::string &key) const
{
    auto it = parameters.find(key);
    if (it != parameters.end()) {
        return it->second;
    }
    return "";
}

Context* Context::Response(boost::beast::http::status status, const std::string &body)
{
    response.result(status);
    response.body() = body;
    response.prepare_payload();
    return this;
}

Context* Context::Parameter(const std::string &key, const std::string &value)
{
    parameters[key] = value;
    return this;
}
