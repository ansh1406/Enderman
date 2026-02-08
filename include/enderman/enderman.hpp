/*
    * Enderman - A simple framework for easily Building HTTP Servers in C++.

    Enderman is a C++ library that provides a simple and intuitive interface for building HTTP servers.
    It is built on top of my indegenous C++ HTTP library "Http-Server" (https://github.com/ansh1406/Http-Server).
    Enderman is inspired by popular Node.js framework Express.js so you'll see many similar looking APIs in Enderman. Like the get and use function.
    This is a completely educational project. I will still treat it as a production ready library in the docs please do not think that I'm trying to glorify it excessively, I'm trying to learn how to write proper user guides and docs.

    All suggestions are welcome.
    If you are also a student and want to contribute to this project, please contact me on my email: anshswaroop14@gmail.com
    We can discuss about the project and how you can contribute to it.
    The project is still in early stages and there are many things to be done.
*/

/// @file enderman.hpp
/// @brief Main header file for the Enderman library. This file includes all the necessary headers workings as a single include.

#ifndef ENDERMAN_HPP
#define ENDERMAN_HPP

#include "types.hpp"
#include "constants.hpp"
#include "request.hpp"
#include "response.hpp"
#include "body.hpp"

#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

/// @brief All the functions and classes of the Enderman library are defined in this namespace.
namespace enderman
{
    /// @brief Main class of the enderman framework. This class provides all the necessary functions to create a server, define routes and middlewares and start the server.
    class Enderman
    {
    private:
        struct Impl;
        Impl *pImpl = nullptr;

    public:
        explicit Enderman();
        ~Enderman();

        /// @brief Register a middleware function for the given path prefixes
        /// @param paths Vector of all path prefixes for which the middleware function should be registered.
        /// @param func Middleware function to be registered for the given path prefixes.
        void use(const std::vector<std::string> &paths, MiddlewareFunction func);
        /// @brief Register a middleware function for the given path prefix
        /// @param path Path prefix for which the middleware function should be registered.
        /// @param func Middleware function to be registered for the given path prefix.
        void use(const std::string &path, MiddlewareFunction func);
        /// @brief Register a middleware function at root.
        void use(MiddlewareFunction func);

        /// @brief Register a route handler for the given HTTP method and path
        /// @param method HTTP method for which the route handler should be registered.
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the given HTTP method and path.
        void on(const enderman::HttpMethod method, const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for the given HTTP method and multiple paths
        /// @param method HTTP method for which the route handler should be registered.
        /// @param paths Vector of all paths for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the given HTTP method and paths.
        void on(const enderman::HttpMethod method, const std::vector<std::string> &paths, RouteHandlerFunction handler);
        /// @brief Register a route handler for the given HTTP methods and path
        /// @param methods Vector of HTTP methods for which the route handler should be registered.
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the given HTTP methods and path.
        void on(const std::vector<enderman::HttpMethod> &methods, const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for the given HTTP methods and multiple paths
        /// @param methods Vector of HTTP methods for which the route handler should be registered.
        /// @param paths Vector of all paths for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the given HTTP methods and paths.
        void on(const std::vector<enderman::HttpMethod> &methods, const std::vector<std::string> &paths, RouteHandlerFunction handler);

        /// @brief Register a route handler for GET method and the given path
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the GET method and the given path.
        void get(const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for GET method and multiple paths
        /// @param paths Vector of all paths for which the route handler should be registered for GET method.
        /// @param handler Route handler function to be registered for GET method and the given paths
        void get(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        /// @brief Register a route handler for POST method and the given path
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the POST method and the given path.
        void post(const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for POST method and multiple paths
        /// @param paths Vector of all paths for which the route handler should be registered for POST method.
        /// @param handler Route handler function to be registered for POST method and the given paths
        void post(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        /// @brief Register a route handler for PUT method and the given path
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the PUT method and the given path.
        void put(const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for PUT method and multiple paths
        /// @param paths Vector of all paths for which the route handler should be registered for PUT method.
        /// @param handler Route handler function to be registered for PUT method and the given paths
        void put(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        /// @brief Register a route handler for DELETE method and the given path
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the DELETE method and the given path.
        void del(const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for DELETE method and multiple paths
        /// @param paths Vector of all paths for which the route handler should be registered for DELETE method.
        /// @param handler Route handler function to be registered for DELETE method and the given paths
        void del(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        /// @brief Register a route handler for PATCH method and the given path
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the PATCH method and the given path.
        void patch(const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for PATCH method and multiple paths
        /// @param paths Vector of all paths for which the route handler should be registered for PATCH method.
        /// @param handler Route handler function to be registered for PATCH method and the given paths
        void patch(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        /// @brief Register a route handler for OPTIONS method and the given path
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the OPTIONS method and the given path.
        void options(const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for OPTIONS method and multiple paths
        /// @param paths Vector of all paths for which the route handler should be registered for OPTIONS method.
        /// @param handler Route handler function to be registered for OPTIONS method and the given paths
        void options(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        /// @brief Register a route handler for HEAD method and the given path
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for the HEAD method and the given path.
        void head(const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for HEAD method and multiple paths
        /// @param paths Vector of all paths for which the route handler should be registered for HEAD method.
        /// @param handler Route handler function to be registered for HEAD method and the given paths
        void head(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        /// @brief Register a route handler for all methods on the given path
        /// @param path Path for which the route handler should be registered.
        /// @param handler Route handler function to be registered for all methods and the given path.
        void any(const std::string &path, RouteHandlerFunction handler);
        /// @brief Register a route handler for all methods on multiple paths
        /// @param paths Vector of all paths for which the route handler should be registered for all methods.
        /// @param handler Route handler function to be registered for all methods and the given paths.
        void any(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        /// @brief Start listening for incoming connections on the given port
        /// @param port Port number on which the server should listen for incoming connections.
        void listen(const unsigned short port);
    };
}

#endif // ENDERMAN_HPP