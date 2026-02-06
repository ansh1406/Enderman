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

    class TextBody : public Body
    {
    public:
        std::string text;
        std::string type;

        TextBody(const std::string &type = "text/plain") : type(type) {}

        void parse_from(const std::vector<char> &body)
        {
            text = std::string(body.begin(), body.end());
        }

        std::vector<char> serialize() const override
        {
            return std::vector<char>(text.begin(), text.end());
        }

        const std::string type() const override
        {
            return type;
        }
    };

    MiddlewareFunction text_body_parser = MiddlewareFunction(
        [](Request &req, Response &res, Next next)
        {
            auto content_type_it = req.headers().find("content-type");
            if (content_type_it != req.headers().end())
            {
                std::string content_type = content_type_it->second;
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

    MiddlewareFunction text_body_parser_forced_parsing = MiddlewareFunction(
        [](Request &req, Response &res, Next next)
        {
            auto content_type_it = req.headers().find("content-type");
            std::string content_type = "text/plain";
            if (content_type_it != req.headers().end())
            {
                content_type = content_type_it->second;
                for (auto &c : content_type)
                    c = std::tolower(c);
            }
            std::shared_ptr<TextBody> text_body = std::make_shared<TextBody>(content_type);
            text_body->parse_from(req.get_body()->as<RawBody>()->data);
            req.set_body(text_body);
            next(nullptr);
        });
}