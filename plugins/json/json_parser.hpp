#include "enderman/request.hpp"
#include "enderman/response.hpp"
#include "enderman/types.hpp"

#include "json_core.hpp"
#include "json_body.hpp"

#include <functional>
#include <stdexcept>
#include <memory>

namespace enderman
{
    auto json_parser = MiddlewareFunction(
        [](Request &req, Response &res, Next next)
        {
            auto content_type_it = req.headers().find("content-type");
            if (content_type_it != req.headers().end() && content_type_it->second == "application/json")
            {
                std::shared_ptr<JsonBody> json_body = std::make_shared<JsonBody>();
                json_body->parse_from(req.get_body()->as<RawBody>()->data);
                req.set_body(json_body);
            }
            next(nullptr);
        });
}