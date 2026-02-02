#ifndef ENDERMAN_MIDDLEWARE_HPP
#define ENDERMAN_MIDDLEWARE_HPP

#include "enderman/types.hpp"

#include <functional>
#include <string>
#include <vector>
#include <stdexcept>
#include <utility>

namespace enderman
{
    struct Middleware
    {
        std::vector<std::string> path;
        MiddlewareFunction func;

        explicit Middleware(const std::vector<std::string> p, MiddlewareFunction f)
            : path(std::move(p)), func(std::move(f)) {}
    };
}

#endif // ENDERMAN_MIDDLEWARE_HPP