#ifndef ENDERMAN_RESPONSE_HPP
#define ENDERMAN_RESPONSE_HPP

#include "types.hpp"

#include <string>
#include <unordered_map>
#include <memory>

namespace enderman
{
    class Response
    {
    private:
        struct Impl;
        struct Impl *pImpl;

    public:

        explicit Response();
        ~Response();

        Response &set_status(int status_code);
        Response &set_message(const std::string &message);
        Response &set_header(const std::string &key, const std::string &value);
        Response &set_body(std::shared_ptr<Body> body);

        void send();
        bool is_sent() const;

        friend class ResponseWriter;
    };
}

#endif // ENDERMAN_RESPONSE_HPP