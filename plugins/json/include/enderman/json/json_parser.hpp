/// @file json_parser.hpp
/// @brief Middleware function to parse JSON request bodies.

#include "enderman/request.hpp"
#include "enderman/response.hpp"
#include "enderman/types.hpp"

#include <enderman_json/json.hpp>
#include "json_body.hpp"

#include <functional>
#include <stdexcept>
#include <memory>

namespace enderman
{
    /// @brief Middleware function to parse JSON request bodies. It checks the Content-Type header of the request and if it matches "application/json", it parses the body as a JsonBody and replaces the original RawBody with the parsed JsonBody in the request.
    auto json_parser = MiddlewareFunction(
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
                if (content_type == "application/json")
                {
                    auto json_body = std::make_shared<JsonBody>();
                    json_body->parse_from(req.get_body()->as<RawBody>()->data);
                    req.set_body(json_body);
                }
            }
            next(nullptr);
        });
}