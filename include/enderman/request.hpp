/// @file request.hpp
/// @brief Defines the Request class for handling HTTP requests in the Enderman library.

#ifndef ENDERMAN_REQUEST_HPP
#define ENDERMAN_REQUEST_HPP

#include "types.hpp"
#include "constants.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace enderman
{
    /// @brief Request class represents an HTTP request received from the client.
    class Request
    {
    private:
        /// @brief IP of the client in x.x.x.x format (Request sender)
        std::string _ip;
        /// @brief Port of client from which client has sent the request
        std::string _port;

        /// @brief HTTP method of the request (GET, POST, etc.).
        HttpMethod _method;

        /// @brief Raw URI of the request as received from the client, including path and query string. Still URL encoded.
        std::string _raw_uri;
        /// @brief Base path of the request, URL decoded and normalized. Fixed for all middlewares and handlers.
        std::string _base_path;
        /// @brief Path of the request relative to the prefix path of the matched route. URL decoded and normalized.
        std::string _relative_path;
        /// @brief Vector of path segments in the base path, URL decoded and normalized.
        std::vector<std::string> _base_path_segments;
        /// @brief Vector of path segments in the relative path, URL decoded and normalized.
        std::vector<std::string> _relative_path_segments;
        /// @brief path parameters extracted from the URI based on the matched route. URL decoded.
        std::unordered_map<std::string, std::string> _path_params;
        /// @brief query parameters extracted from the URI. URL decoded.
        std::unordered_map<std::string, std::string> _query_params;

        /// @brief headers of the request. Header names are normalized to lowercase. Header values are not modified.
        const std::unordered_map<std::string, std::string> _headers;

        /// @brief Request Body as a shared pointer to an object of a class that inherits from Body. Initially if request has no body, it is set to nullptr and if request has a body, it is set to a shared pointer to a RawBody object containing the raw body data.
        std::shared_ptr<Body> body;

    public:
        /// @brief Constructor for Request class.
        /// @param ip IP address of the client in x.x.x.x format.
        /// @param port Port number from which the client has sent the request.
        /// @param method HTTP method of the request as an HttpMethod enum value.
        /// @param raw_uri Raw URI of the request as received from the client, including path and query string. Still URL encoded.
        /// @param headers Headers of the request. Normalize header names to lowercase before passing to this constructor.
        Request(const std::string &ip,
                const std::string &port,
                const HttpMethod method,
                const std::string &raw_uri,
                const std::unordered_map<std::string, std::string> &headers)
            : _ip(ip),
              _port(port),
              _method(method),
              _headers(headers),
              _raw_uri(raw_uri) {}

        ~Request() = default;
        /// @brief Get the IP address of the client
        /// @return IP address of the client in x.x.x.x format as std::string
        const std::string &ip() const { return _ip; }
        /// @brief Get the Port from which the client has sent the request
        /// @return Port number as std::string
        const std::string &port() const { return _port; }
        /// @brief Get the HTTP method of the request.
        /// @return HTTP method as an HttpMethod enum value.
        HttpMethod method() const { return _method; }
        /// @brief Get the raw URI of the request as received from the client, including path and query string. Still URL encoded.
        /// @return Raw URI as a string.
        const std::string &raw_uri() const { return _raw_uri; }
        /// @brief Get the base path of the request, URL decoded and normalized. Fixed for all middlewares and handlers.
        /// @return Base path as a string.
        const std::string &base_path() const { return _base_path; }
        /// @brief Get the path of the request relative to the prefix path of the matched route. URL decoded and normalized.
        /// @return Relative path as a string.
        const std::string &relative_path() const { return _relative_path; }
        /// @brief Get the vector of path segments in the base path, URL decoded and normalized.
        /// @return Vector of path segments in the base path.
        const std::vector<std::string> &base_path_segments() const { return _base_path_segments; }
        /// @brief Get the vector of path segments in the relative path, URL decoded and normalized.
        /// @return Vector of path segments in the relative path.
        const std::vector<std::string> &relative_path_segments() const { return _relative_path_segments; }
        /// @brief Get the path parameters extracted from the URI based on the matched route. URL decoded.
        /// @return Path parameters as an unordered map.
        const std::unordered_map<std::string, std::string> &path_params() const { return _path_params; }
        /// @brief Get the query parameters extracted from the URI. URL decoded.
        /// @return Query parameters as an unordered map.
        const std::unordered_map<std::string, std::string> &query_params() const { return _query_params; }
        /// @brief Get the headers of the request. Header names are normalized to lowercase. Header values are not modified.
        /// @return Headers as an unordered map.
        const std::unordered_map<std::string, std::string> &headers() const { return _headers; }
        /// @brief Get the request body as a shared pointer to an object of a class that inherits from Body.
        /// @return Shared pointer to the request body.
        std::shared_ptr<Body> get_body() const { return body; }
        /// @brief Set the request body as a shared pointer to an object of a class that inherits from Body.
        /// @param body_ptr Shared pointer to the request body.
        void set_body(std::shared_ptr<Body> body_ptr) { body = body_ptr; }
        /// @brief Check if the request has a body.
        /// @return True if the request has a body, false otherwise.
        bool has_body() const { return body != nullptr; }

        friend class RequestBuilder;
    };
}

#endif // ENDERMAN_REQUEST_HPP