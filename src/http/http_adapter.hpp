#pragma once

#include "http/includes/http.hpp"

#include "includes/types.hpp"

#include <string>
#include <functional>
#include <stdexcept>

namespace enderman
{
    namespace http
    {
        class HttpAdapter
        {
        private:
            struct Impl;
            Impl *pImpl = nullptr;

        public:
            explicit HttpAdapter() = default;
            ~HttpAdapter();

            void create_server(unsigned short int port, EndermanCallbackFunction &handler);
            void start_server();

            struct UnableToCreateServerException : public std::runtime_error
            {
                explicit UnableToCreateServerException(const std::string &message)
                    : std::runtime_error(message) {}
            };

            struct HttpServerInternalError : public std::runtime_error
            {
                explicit HttpServerInternalError(const std::string &message)
                    : std::runtime_error(message) {}
            };
        };
    }
}