/// @file types.hpp
/// @brief Defines forward declarations and type aliases for the Enderman library.

#ifndef ENDERMAN_TYPES_HPP
#define ENDERMAN_TYPES_HPP

#include <functional>
#include <stdexcept>

namespace enderman
{
    class Request;
    class Response;
    class Body;

    /// @brief Function type for middleware to call next middleware in the chain. It accepts a std::exception_ptr which can pass the error back to main flow of the framework. Passing nullptr means no error.
    using Next = std::function<void(std::exception_ptr)>;
    /// @brief Function type for middleware. It accepts a Request, Response and a Next function to call the next middleware in the chain.
    using MiddlewareFunction = std::function<void(Request &, Response &, const Next &)>;
    /// @brief Function type for route handlers. It accepts a Request and Response.
    using RouteHandlerFunction = std::function<void(Request &, Response &)>;
}

#endif // ENDERMAN_TYPES_HPP