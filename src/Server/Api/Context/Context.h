#pragma once
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <map>

class Context {
private:
    const boost::beast::http::request<boost::beast::http::string_body> &request;
    boost::beast::http::response<boost::beast::http::string_body> &response;
    std::map<std::string, std::string> parameters;

public:
    Context(
        const boost::beast::http::request<boost::beast::http::string_body> &req,
        boost::beast::http::response<boost::beast::http::string_body> &res
    ) : 
        request(req),
        response(res) { }

    const Context* get() const {
        return this;
    }

    Context* set() {
        return this;
    }

    const boost::beast::http::request<boost::beast::http::string_body>& Request() const;
    boost::beast::http::response<boost::beast::http::string_body>& Response();
    std::string Parameter(const std::string &key) const;

    Context* Response(boost::beast::http::status status, const std::string &body);
    Context* Parameter(const std::string &key, const std::string &value);
};