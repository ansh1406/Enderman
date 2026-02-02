#ifndef ENDERMAN_HPP
#define ENDERMAN_HPP

#include "enderman/types.hpp"
#include "enderman/constants.hpp"

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
    };
}

#endif // ENDERMAN_HPP