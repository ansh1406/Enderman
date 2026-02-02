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