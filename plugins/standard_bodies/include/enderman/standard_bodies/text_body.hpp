/// @file text_body.hpp
/// @brief Derived class for text type bodies and middleware to parse them.

#pragma once

#include "enderman/request.hpp"
#include "enderman/response.hpp"
#include "enderman/types.hpp"
#include "enderman/body.hpp"

#include <string>
#include <cctype>
#include <unordered_set>
#include <memory>

namespace enderman
{
    /// @brief Types supported by the TextBody parser. This is not an exhaustive list, but it covers common text types. You can add more types to this set as needed.
    std::unordered_set<std::string> text_types = {
        "text/plain",
        "text/html",
        "text/css",
        "text/javascript",
        "text/csv",
        "text/xml",
        "text/markdown",
        "application/xml",
        "application/xhtml+xml",
        "application/javascript",
        "application/json",
    };

    /// @brief TextBody class that inherits from Body and represents text-based request bodies.
    class TextBody : public Body
    {
    public:
        std::string text;
        std::string content_type;

        TextBody(const std::string &content_type = "text/plain") : content_type(content_type) {}

        void parse_from(const std::vector<char> &body)
        {
            text = std::string(body.begin(), body.end());
        }

        std::vector<char> serialize() const override
        {
            return std::vector<char>(text.begin(), text.end());
        }

        const std::string type() const override { return content_type; }
    };

    /// @brief Middleware function to parse text-based request bodies. It checks the Content-Type header of the request and if it matches one of the supported text types, it parses the body as a TextBody and replaces the original RawBody with the parsed TextBody in the request.
    MiddlewareFunction text_body_parser = MiddlewareFunction(
        [](Request &req, Response &res, Next next)
        {
            auto content_type_it = req.headers().find("content-type");
            if (content_type_it != req.headers().end())
            {
                std::string content_type = content_type_it->second;
                content_type = content_type_it->second;
                auto sem = content_type.find(';');
                if (sem != std::string::npos)
                    content_type = content_type.substr(0, sem);
                for (auto &c : content_type)
                    c = std::tolower(c);
                if (text_types.find(content_type) != text_types.end())
                {
                    std::shared_ptr<TextBody> text_body = std::make_shared<TextBody>(content_type);
                    text_body->parse_from(req.get_body()->as<RawBody>()->data);
                    req.set_body(text_body);
                }
            }
            next(nullptr);
        });

    /// @brief Middleware function to force parsing of request bodies as TextBody regardless of the Content-Type header. This can be useful in cases where the client does not set the Content-Type header correctly or when you want to treat all request bodies as text for a specific route or group of routes.
    MiddlewareFunction text_body_parser_forced_parsing = MiddlewareFunction(
        [](Request &req, Response &res, Next next)
        {
            auto content_type_it = req.headers().find("content-type");
            std::string content_type = "text/plain";
            if (content_type_it != req.headers().end())
            {
                content_type = content_type_it->second;
                auto sem = content_type.find(';');
                if (sem != std::string::npos)
                    content_type = content_type.substr(0, sem);
                for (auto &c : content_type)
                    c = std::tolower(c);
            }
            std::shared_ptr<TextBody> text_body = std::make_shared<TextBody>(content_type);
            text_body->parse_from(req.get_body()->as<RawBody>()->data);
            req.set_body(text_body);
            next(nullptr);
        });
}