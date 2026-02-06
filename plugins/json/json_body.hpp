#pragma once
#include "json_core.hpp"
#include "enderman/body.hpp"

namespace enderman
{
    class JsonBody : public enderman::Body
    {
    private:
        enderman_json::Object jsonobj;
        static constexpr const char *TYPE = "application/json";

    public:
        JsonBody(enderman_json::Object obj = enderman_json::Object()) : jsonobj(obj) {}
        void parse_from(const std::vector<char> &body)
        {
            std::string str(body.begin(), body.end());
            jsonobj = enderman_json::parse_json(str);
        }
        std::vector<char> serialize() const override
        {
            std::string str = enderman_json::stringify(jsonobj);
            return std::vector<char>(str.begin(), str.end());
        }
        const std::string type() const override { return std::string(TYPE); }
    };
}