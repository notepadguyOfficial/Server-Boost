#include "Routes.h"

Routes* Routes::add(boost::beast::http::verb method, const std::string &pattern, std::function<void(Context*)> handler)
{
    routes.push_back(
        {
            method, 
            pattern,
            handler
        }
    );

    return this;
}

bool Routes::route(Context* ctx)
{
    const auto &req = ctx->get()->Request();
    std::string target = std::string(req.target());

    if (!prefix.empty() && target.find(prefix) == 0)
    {
        target.erase(0, prefix.size());
    }

    if (!target.empty() && target.back() == '/')
    {
        target.pop_back();
    }

    for (const auto &info : routes)
    {
        if (req.method() != info.method)
            continue;
        
        std::vector<std::string> target_segments = split(target);
        std::vector<std::string> pattern_segments = split(info.pattern);

        if (target_segments.size() != pattern_segments.size())
            continue;
        
        bool match = true;

        for (size_t i = 0; i < pattern_segments.size(); ++i)
        {
            if (pattern_segments[i].front() == '{'
                && pattern_segments[i].back() == '}')
            {
                std::string param = pattern_segments[i]
                    .substr(
                        1,
                        pattern_segments[i].length() - 2);
                ctx->set()
                    ->Parameter(
                        param,
                        target_segments[i]
                    );
            }
            else if (pattern_segments[i] != target_segments[i])
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            info.handler(ctx);
            return true;
        }
    }

    return false;
}

Routes* Routes::Prefix(const std::string &p)
{
    this->prefix = p;

    if (!this->prefix.empty() && this->prefix.back() != '/')
    {
        this->prefix += '/';   
    }

    return this;
}

std::vector<std::string> Routes::split(const std::string &path)
{
    std::vector<std::string> segments;
    size_t start = 0, end = 0;

    while ((end = path.find('/', start)) != std::string::npos)
    {
        if (end != start)
            segments.push_back(path.substr(start, end - start));

        start = end + 1;
    }

    if (start < path.size())
        segments.push_back(path.substr(start));

    return segments;
}
