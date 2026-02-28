/// @file json_body.hpp
/// @brief Derived class for JSON type bodies.

#pragma once
#include <enderman_json/json.hpp>
#include "enderman/body.hpp"
#include "enderman/request.hpp"
#include "enderman/response.hpp"

#include <memory>

namespace enderman
{
    /// @brief JsonBody class that inherits from Body and represents JSON request or response bodies.
    class JsonBody : public enderman::Body
    {
    public:
        enderman_json::Object jsonobj;

        JsonBody(enderman_json::Object obj = enderman_json::Object()) : jsonobj(obj) {}

        void parse_from(const std::vector<char> &body)
        {
            std::string str(body.begin(), body.end());
            parse_from(str);
        }

        void parse_from(const std::string &body)
        {
            jsonobj = enderman_json::parse_json(body);
        }

        std::vector<char> serialize() const override
        {
            std::string str = enderman_json::stringify(jsonobj);
            return std::vector<char>(str.begin(), str.end());
        }

        const std::string type() const override { return std::string("application/json"); }

        /// @brief Creates a new JsonBody with the given JSON object and sets it as the body of the response.
        /// @param res The response object to set the body on.
        /// @param obj The JSON object to set in the body.
        /// @return Reference to the response object with the new body set.
        static Response &set_json(Response &res, enderman_json::Object obj)
        {
            auto body = std::make_shared<JsonBody>(obj);
            res.set_body(body);
            return res;
        }
    };
}