#ifndef ENDERMAN_ROUTE_HANDLER_HPP
#define ENDERMAN_ROUTE_HANDLER_HPP

#include "enderman/types.hpp"

#include <string>
#include <vector>
#include <functional>
#include <utility>

namespace enderman
{
    struct RouteHandler
    {
        std::vector<std::string> path;
        RouteHandlerFunction handler;
        explicit RouteHandler(const std::vector<std::string> _path, RouteHandlerFunction f)
            : path(std::move(_path)), handler(std::move(f))
        {
        }
    };
}

#endif // ENDERMAN_ROUTE_HANDLER_HPP
