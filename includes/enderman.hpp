#ifndef ENDERMAN_HPP
#define ENDERMAN_HPP

#include "types.hpp"
#include "constants.hpp"

#include <functional>
#include <stdexcept>
#include <string>
#include <vector>

namespace enderman
{
    class Enderman
    {
    private:
        struct Impl;
        Impl *pImpl = nullptr;

    public:
        explicit Enderman();
        ~Enderman();

        void use(const std::vector<std::string> &paths, MiddlewareFunction func);
        void use(const std::string &path, MiddlewareFunction func);
        void use(MiddlewareFunction func);

        void on(const enderman::HttpMethod method, const std::string &path, RouteHandlerFunction handler);
        void on(const enderman::HttpMethod method, const std::vector<std::string> &paths, RouteHandlerFunction handler);
        void on(const std::vector<enderman::HttpMethod> &methods, const std::string &path, RouteHandlerFunction handler);
        void on(const std::vector<enderman::HttpMethod> &methods, const std::vector<std::string> &paths, RouteHandlerFunction handler);

        void get(const std::string &path, RouteHandlerFunction handler);
        void get(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        void post(const std::string &path, RouteHandlerFunction handler);
        void post(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        void put(const std::string &path, RouteHandlerFunction handler);
        void put(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        void del(const std::string &path, RouteHandlerFunction handler);
        void del(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        void patch(const std::string &path, RouteHandlerFunction handler);
        void patch(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        void options(const std::string &path, RouteHandlerFunction handler);
        void options(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        void head(const std::string &path, RouteHandlerFunction handler);
        void head(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        void any(const std::string &path, RouteHandlerFunction handler);
        void any(const std::vector<std::string> &paths, RouteHandlerFunction handler);

        void listen(const unsigned short port);
    };
}

#endif // ENDERMAN_HPP