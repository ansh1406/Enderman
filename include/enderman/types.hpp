#ifndef ENDERMAN_TYPES_HPP
#define ENDERMAN_TYPES_HPP

#include <functional>
#include <stdexcept>

namespace enderman
{
    class Request;
    class Response;
    class Body;

    using Next = std::function<void(std::exception_ptr)>;
    using MiddlewareFunction = std::function<void(Request &, Response &, const Next &)>;
    using RouteHandlerFunction = std::function<void(Request &, Response &)>;
}

#endif // ENDERMAN_TYPES_HPP