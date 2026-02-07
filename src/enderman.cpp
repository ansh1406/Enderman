#include "enderman/types.hpp"
#include "enderman/constants.hpp"
#include "enderman/enderman.hpp"
#include "enderman/request.hpp"
#include "enderman/response.hpp"

#include "http/http_adapter.hpp"

#include "utils.hpp"
#include "middleware.hpp"
#include "route_handler.hpp"
#include "request_builder.hpp"

#include <functional>
#include <stdexcept>
#include <vector>
#include <utility>
#include <unordered_map>
#include <iostream>

namespace enderman
{
    struct Enderman::Impl
    {
        std::vector<Middleware> middlewares;
        std::unordered_map<enderman::HttpMethod, std::vector<RouteHandler>> route_handlers;

        void run_middlewares(Request &req, Response &res);
        void run_route_handler(Request &req, Response &res);
        void build_request(Request &req);
    };
}

enderman::Enderman::Enderman() : pImpl(new Impl()) {}

enderman::Enderman::~Enderman()
{
    delete pImpl;
}

void enderman::Enderman::use(const std::string &path, MiddlewareFunction func)
{
    auto segments = enderman::utils::UriParser::parse_path(path);
    pImpl->middlewares.push_back(Middleware({segments}, std::move(func)));
}

void enderman::Enderman::use(const std::vector<std::string> &paths, MiddlewareFunction func)
{
    for (const auto &path : paths)
    {
        use(path, func);
    }
}

void enderman::Enderman::use(MiddlewareFunction func)
{
    pImpl->middlewares.push_back(Middleware({}, std::move(func)));
}

void enderman::Enderman::on(const enderman::HttpMethod method, const std::string &path, RouteHandlerFunction handler)
{
    auto segments = enderman::utils::UriParser::parse_path(path);
    pImpl->route_handlers[method].emplace_back(segments, std::move(handler));
}

void enderman::Enderman::on(const enderman::HttpMethod method, const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    for (const auto &path : paths)
    {
        on(method, path, handler);
    }
}

void enderman::Enderman::on(const std::vector<enderman::HttpMethod> &methods, const std::string &path, RouteHandlerFunction handler)
{
    for (const auto &method : methods)
    {
        on(method, path, handler);
    }
}

void enderman::Enderman::on(const std::vector<enderman::HttpMethod> &methods, const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    for (const auto &method : methods)
    {
        on(method, paths, handler);
    }
}

void enderman::Enderman::get(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::GET, path, std::move(handler));
}

void enderman::Enderman::get(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::GET, paths, std::move(handler));
}

void enderman::Enderman::post(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::POST, path, std::move(handler));
}

void enderman::Enderman::post(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::POST, paths, std::move(handler));
}

void enderman::Enderman::put(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::PUT, path, std::move(handler));
}

void enderman::Enderman::put(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::PUT, paths, std::move(handler));
}

void enderman::Enderman::del(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::DELETE, path, std::move(handler));
}

void enderman::Enderman::del(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::DELETE, paths, std::move(handler));
}

void enderman::Enderman::patch(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::PATCH, path, std::move(handler));
}

void enderman::Enderman::patch(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::PATCH, paths, std::move(handler));
}

void enderman::Enderman::options(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::OPTIONS, path, std::move(handler));
}

void enderman::Enderman::options(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::OPTIONS, paths, std::move(handler));
}

void enderman::Enderman::head(const std::string &path, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::HEAD, path, std::move(handler));
}

void enderman::Enderman::head(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    on(enderman::HttpMethod::HEAD, paths, std::move(handler));
}

void enderman::Enderman::any(const std::string &path, RouteHandlerFunction handler)
{
    std::vector<enderman::HttpMethod> methods = {
        enderman::HttpMethod::GET,
        enderman::HttpMethod::POST,
        enderman::HttpMethod::PUT,
        enderman::HttpMethod::DELETE,
        enderman::HttpMethod::PATCH,
        enderman::HttpMethod::OPTIONS,
        enderman::HttpMethod::HEAD};

    on(methods, path, std::move(handler));
}

void enderman::Enderman::any(const std::vector<std::string> &paths, RouteHandlerFunction handler)
{
    for (auto &path : paths)
    {
        any(path, handler);
    }
}

void enderman::Enderman::listen(const unsigned short port)
{
    enderman::http::HttpAdapter http_adapter;
    try
    {
        EndermanCallbackFunction handler = [this](Request &req, Response &res)
        {
            try
            {
                pImpl->build_request(req);
                pImpl->run_middlewares(req, res);
                if (res.is_sent())
                {
                    return;
                }
                pImpl->run_route_handler(req, res);
            }
            catch (std::exception &e)
            {
                std::cerr << "Error processing request: " << e.what() << std::endl;
                res.set_status(500).set_body(nullptr).send();
            }
        };
        http_adapter.create_server(port, handler);
    }
    catch (const enderman::http::HttpAdapter::UnableToCreateServerException &e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
    try
    {
        http_adapter.start_server();
    }
    catch (const enderman::http::HttpAdapter::HttpServerInternalError &e)
    {
        std::cerr << e.what() << std::endl;
        return;
    }
}

void enderman::Enderman::Impl::build_request(Request &req)
{
    auto parsed_uri = enderman::utils::UriParser::parse_uri(req.raw_uri());
    RequestBuilder::set_base_path_segments(req, parsed_uri.path_segments);
    RequestBuilder::set_base_path(req, enderman::utils::PathTools::build_path(parsed_uri.path_segments));
    RequestBuilder::set_query_params(req, parsed_uri.query_params);
}

void enderman::Enderman::Impl::run_middlewares(Request &req, Response &res)
{
    size_t index = 0;
    enderman::Next next = [&](std::exception_ptr e)
    {
        if (res.is_sent())
        {
            return;
        }

        if (e)
        {
            try
            {
                std::rethrow_exception(e);
            }
            catch (const std::exception &ex)
            {
                std::cerr << "Error received from middleware: " << ex.what() << std::endl;
            }
            res.set_status(500).set_body(nullptr).send();
            return;
        }

        while (index < middlewares.size())
        {
            Middleware &mw = middlewares[index++];
            if (enderman::utils::PathTools::match_prefix(req.base_path_segments(), mw.path))
            {
                auto path_params = enderman::utils::PathTools::extract_path_params(req.base_path_segments(), mw.path);
                RequestBuilder::set_path_params(req, path_params);
                RequestBuilder::set_relative_path_segments(req, enderman::utils::PathTools::get_relative_path(req.base_path_segments(), mw.path));
                RequestBuilder::set_relative_path(req, enderman::utils::PathTools::build_path(req.relative_path_segments()));
                mw.func(req, res, next);
                return;
            }
        }
    };
    next(nullptr);
}

void enderman::Enderman::Impl::run_route_handler(Request &req, Response &res)
{
    try
    {
        auto it = route_handlers.find(req.method());
        if (it != route_handlers.end())
        {
            for (const auto &route_handler : it->second)
            {
                if (enderman::utils::PathTools::match_full_path(req.base_path_segments(), route_handler.path))
                {
                    auto path_params = enderman::utils::PathTools::extract_path_params(req.base_path_segments(), route_handler.path);
                    RequestBuilder::set_path_params(req, path_params);
                    RequestBuilder::set_relative_path_segments(req, enderman::utils::PathTools::get_relative_path(req.base_path_segments(), route_handler.path));
                    RequestBuilder::set_relative_path(req, enderman::utils::PathTools::build_path(req.relative_path_segments()));
                    route_handler.handler(req, res);
                    return;
                }
            }
        }
        res.set_status(404).set_body(nullptr).send();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error in route handler: " << e.what() << std::endl;
        res.set_status(500).set_body(nullptr).send();
    }
    catch (...)
    {
        std::cerr << "Unknown error in route handler" << std::endl;
        res.set_status(500).set_body(nullptr).send();
    }
}