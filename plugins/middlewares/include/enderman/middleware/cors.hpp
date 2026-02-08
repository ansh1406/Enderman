/// @file cors.hpp
/// @brief Middleware function to handle Cross-Origin Resource Sharing (CORS) in Enderman.

#pragma once

#include "enderman/types.hpp"
#include "enderman/request.hpp"
#include "enderman/response.hpp"

#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <algorithm>

namespace
{
    std::string join(const std::vector<std::string> &vec, const std::string &delimiter)
    {
        std::ostringstream oss;
        for (size_t i = 0; i < vec.size(); ++i)
        {
            oss << vec[i];
            if (i < vec.size() - 1)
            {
                oss << delimiter;
            }
        }
        return oss.str();
    }
}

namespace enderman
{
    /// @brief Configuration struct for CORS middleware.
    /// @param allowed_origins List of allowed origins. If empty, all origins are allowed.
    /// @param allowed_methods List of allowed HTTP methods. If empty, all methods are allowed
    /// @param allowed_headers List of allowed HTTP headers. If empty, all headers are allowed.
    /// @param allow_credentials Whether to allow credentials (cookies, authorization headers, etc.). False by default.
    struct CorsConfig
    {
        std::vector<std::string> allowed_origins;
        std::vector<std::string> allowed_methods;
        std::vector<std::string> allowed_headers;
        bool allow_credentials = false;
    };

    /// @brief Middleware function generator to handle Cross-Origin Resource Sharing (CORS) in Enderman.
    /// It takes a CorsConfig struct as an argument and returns a MiddlewareFunction that can be used in the Enderman middleware stack to handle CORS for incoming requests.
    /// @param config Configuration for CORS handling, including allowed origins, methods, headers, and whether to allow credentials.
    /// @return MiddlewareFunction that can be used in the Enderman middleware stack to handle CORS for incoming requests based on the provided configuration.
    MiddlewareFunction cors(const CorsConfig &config = CorsConfig{})
    {
        return [config](Request &req, Response &res, const Next &next)
        {
            if (req.headers().find("origin") == req.headers().end())
            {
                next(nullptr);
                return;
            }
            std::string origin = req.headers().at("origin");

            bool origin_allowed = false;
            if (config.allowed_origins.empty())
            {
                origin_allowed = true;
            }
            else
            {
                origin_allowed = std::find(config.allowed_origins.begin(), config.allowed_origins.end(), origin) != config.allowed_origins.end();
            }

            if (!origin_allowed)
            {
                next(nullptr);
                return;
            }

            if (config.allow_credentials)
            {
                res.set_header("Access-Control-Allow-Origin", origin);
                res.set_header("Vary", "Origin");
            }
            else if (config.allowed_origins.empty())
            {
                res.set_header("Access-Control-Allow-Origin", "*");
            }
            else
            {
                res.set_header("Access-Control-Allow-Origin", origin);
                res.set_header("Vary", "Origin");
            }

            if (!config.allowed_methods.empty())
            {
                res.set_header("Access-Control-Allow-Methods", join(config.allowed_methods, ","));
            }

            if (!config.allowed_headers.empty())
            {
                res.set_header("Access-Control-Allow-Headers", join(config.allowed_headers, ","));
            }

            if (config.allow_credentials)
            {
                res.set_header("Access-Control-Allow-Credentials", "true");
            }

            if (req.method() == HttpMethod::OPTIONS)
            {
                res.set_status(204).send();
                return;
            }

            next(nullptr);
        };
    }
}