#include "enderman/types.hpp"
#include "enderman/constants.hpp"
#include "enderman/enderman.hpp"
#include "enderman/utils.hpp"
#include "enderman/middleware.hpp"
#include "enderman/route_handler.hpp"

#include <functional>
#include <stdexcept>
#include <vector>
#include <utility>
#include <unordered_map>

namespace enderman
{
    struct Enderman::Impl
    {
        std::vector<Middleware> middlewares;
        std::unordered_map<enderman::HttpMethod, std::vector<RouteHandler>> routes;
    };
}

enderman::Enderman::Enderman() : pImpl(new Impl()) {}

enderman::Enderman::~Enderman()
{
    delete pImpl;
}

void enderman::Enderman::use(const std::string &path, MiddlewareFunction func)
{
    auto segments = enderman::utils::UriParser::parse_path(path);
    pImpl->middlewares.push_back(Middleware({segments}, std::move(func)));
}

void enderman::Enderman::use(const std::vector<std::string> &paths, MiddlewareFunction func)
{
    for (const auto &path : paths)
    {
        use(path, func);
    }
}

void enderman::Enderman::use(MiddlewareFunction func)
{
    pImpl->middlewares.push_back(Middleware({}, std::move(func)));
}

void enderman::Enderman::on(const enderman::HttpMethod method, const std::string &path, RouteHandlerFunction handler)
{
    auto segments = enderman::utils::UriParser::parse_path(path);
    pImpl->routes[method].emplace_back(segments, std::move(handler));
}

void enderman::Enderman::on(const enderman::HttpMethod method, const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    for (const auto &path : paths)
    {
        on(method, path, handler);
    }
}

void enderman::Enderman::on(const std::vector<enderman::HttpMethod> &methods, const std::string &path, RouteHandlerFunction handler)
{
    for (const auto &method : methods)
    {
        on(method, path, handler);
    }
}

void enderman::Enderman::on(const std::vector<enderman::HttpMethod> &methods, const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    for (const auto &method : methods)
    {
        on(method, paths, handler);
    }
}

void enderman::Enderman::get(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::GET, path, std::move(handler));
}

void enderman::Enderman::get(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::GET, paths, std::move(handler));
}

void enderman::Enderman::post(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::POST, path, std::move(handler));
}

void enderman::Enderman::post(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::POST, paths, std::move(handler));
}

void enderman::Enderman::put(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::PUT, path, std::move(handler));
}

void enderman::Enderman::put(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::PUT, paths, std::move(handler));
}

void enderman::Enderman::del(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::DELETE, path, std::move(handler));
}

void enderman::Enderman::del(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::DELETE, paths, std::move(handler));
}

void enderman::Enderman::patch(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::PATCH, path, std::move(handler));
}

void enderman::Enderman::patch(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::PATCH, paths, std::move(handler));
}

void enderman::Enderman::options(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::OPTIONS, path, std::move(handler));
}

void enderman::Enderman::options(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::OPTIONS, paths, std::move(handler));
}

void enderman::Enderman::head(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::HEAD, path, std::move(handler));
}

void enderman::Enderman::head(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::HEAD, paths, std::move(handler));
}
