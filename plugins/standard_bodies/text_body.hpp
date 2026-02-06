#pragma once

#include "enderman/body.hpp"

#include <string>

namespace enderman
{
    class TextBody : public Body
    {
    public:
        std::string text;
        std::string type;

        TextBody(const std::string &type = "text/plain") : type(type) {}

        void parse_from(const std::vector<char> &body) override
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
}