#ifndef ENDERMAN_HPP
#define ENDERMAN_HPP

#include "enderman/types.hpp"

#include <functional>
#include <stdexcept>
#include <string>

namespace enderman
{
    class Enderman
    {
    private:
        struct Impl;
        Impl *pImpl = nullptr;

    public:
        Enderman();

        void use(const std::vector<std::string> &paths, MiddlewareFunction func);
        void use(const std::string &path, MiddlewareFunction func);
        void use(MiddlewareFunction func);
    };
}

#endif // ENDERMAN_HPP