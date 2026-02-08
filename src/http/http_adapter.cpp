#include "enderman/types.hpp"
#include "enderman/request.hpp"
#include "enderman/response.hpp"
#include "enderman/body.hpp"
#include "enderman/constants.hpp"

#include "../response_writer.hpp"

#include "http_adapter.hpp"

#include <string>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <memory>

enderman::HttpMethod get_enderman_method(const std::string &method_str)
{
    if (method_str == "GET")
        return enderman::HttpMethod::GET;
    else if (method_str == "POST")
        return enderman::HttpMethod::POST;
    else if (method_str == "PUT")
        return enderman::HttpMethod::PUT;
    else if (method_str == "DELETE")
        return enderman::HttpMethod::DELETE;
    else if (method_str == "PATCH")
        return enderman::HttpMethod::PATCH;
    else if (method_str == "OPTIONS")
        return enderman::HttpMethod::OPTIONS;
    else if (method_str == "HEAD")
        return enderman::HttpMethod::HEAD;
    else
        throw std::invalid_argument("Unsupported HTTP method: " + method_str);
}

enderman::Request convert_http_request_to_enderman_request(const ::http::HttpRequest &http_request)
{
    enderman::HttpMethod method = get_enderman_method(http_request.method);
    enderman::Request enderman_request(method,
                                       http_request.uri,
                                       std::unordered_map<std::string, std::string>(http_request.headers.begin(), http_request.headers.end()));
    if (!http_request.body.empty())
    {
        std::shared_ptr<enderman::RawBody> body = std::make_shared<enderman::RawBody>();
        body->parse_from(http_request.body);
        enderman_request.set_body(body);
    }
    return enderman_request;
}

void write_enderman_response_to_http_response(const enderman::Response &enderman_response, ::http::HttpResponse &http_response)
{
    http_response.set_status_code(enderman::ResponseWriter::get_status_code(enderman_response));
    std::string reason_phrase = enderman::ResponseWriter::get_reason_phrase(enderman_response);
    if (reason_phrase.empty())
        reason_phrase = " ";
    http_response.set_status_message(reason_phrase);
    auto headers = enderman::ResponseWriter::get_headers(enderman_response);
    for (const auto &header : headers)
    {
        http_response.add_header(header.first, header.second);
    }
    http_response.set_body(enderman::ResponseWriter::get_body(enderman_response));
    if (http_response.body().size() > 0)
    {
        http_response.add_header("Content-Length", std::to_string(http_response.body().size()));
    }
}

struct enderman::http::HttpAdapter::Impl
{
    ::http::HttpServer server;

    Impl(::http::HttpServerConfig config, std::function<void(const ::http::HttpRequest &, ::http::HttpResponse &)> func)
        : server(config, func) {}
};

enderman::http::HttpAdapter::~HttpAdapter()
{
    delete pImpl;
}

void enderman::http::HttpAdapter::create_server(unsigned short int port, EndermanCallbackFunction &handler)
{
    ::http::HttpServerConfig config{
        .port = port,
        .max_pending_connections = 128,
        .max_concurrent_connections = 128,
        .inactive_connection_timeout_in_seconds = 60,
        .enable_logging = false};

    auto http_handler = [handler](const ::http::HttpRequest &req, ::http::HttpResponse &res)
    {
        try
        {
            enderman::Request enderman_request = convert_http_request_to_enderman_request(req);
            enderman::Response enderman_response;
            handler(enderman_request, enderman_response);
            write_enderman_response_to_http_response(enderman_response, res);
        }
        catch (...)
        {
            res.set_status_code(500);
            res.set_status_message("Internal Server Error");
        }
    };
    try
    {
        pImpl = new Impl(config, std::move(http_handler));
    }
    catch (const std::exception &e)
    {
        throw enderman::http::HttpAdapter::UnableToCreateServerException("Unable to create HTTP server: " + std::string(e.what()));
    }
    catch (...)
    {
        throw enderman::http::HttpAdapter::UnableToCreateServerException("Unable to create HTTP server: Unknown error");
    }
}

void enderman::http::HttpAdapter::start_server()
{
    if (pImpl == nullptr)
    {
        throw enderman::http::HttpAdapter::HttpServerInternalError("HTTP server not created. Call create_server() before starting the server.");
    }
    try
    {
        pImpl->server.start();
    }
    catch (const std::exception &e)
    {
        throw enderman::http::HttpAdapter::HttpServerInternalError("Unable to start HTTP server: " + std::string(e.what()));
    }
    catch (...)
    {
        throw enderman::http::HttpAdapter::HttpServerInternalError("Unable to start HTTP server: Unknown error");
    }
}