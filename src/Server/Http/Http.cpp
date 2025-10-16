#include "Http.h"
#include "Logs.h"

Http* Http::session(boost::asio::ip::tcp::socket socket)
{
    try {
        boost::beast::flat_buffer buffer;
        boost::beast::http::request<boost::beast::http::string_body> req;
        boost::beast::http::read(socket, buffer, req);
        boost::beast::http::response<boost::beast::http::string_body> res;
        auto ctx = Context(req, res);

        if (!routes->route(&ctx))
        {
            res.result(boost::beast::http::status::not_found);
            res.body() = "Resource not found";
        }

        if (res.body().length() != 0)
            res.prepare_payload();
        
        boost::beast::http::write(socket, res);
    }
    catch (const std::exception& e)
    {
        LOG_ERROR(Protocol::Http, "{}", e.what());
    }

    return this;
}

Http* Http::run()
{
    boost::asio::ip::tcp::acceptor acceptor{
        ioc, 
        {
            boost::asio::ip::tcp::v4(),
            static_cast<boost::asio::ip::port_type>(port)
        }
    };

    for (;;)
    {
        boost::asio::ip::tcp::socket socket{ioc};
        acceptor.accept(socket);
        std::thread(&Http::session, this, std::move(socket)).detach();
    }

    return this;
}

short Http::Port() const
{
    return port;
}
