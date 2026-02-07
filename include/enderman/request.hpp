#ifndef ENDERMAN_REQUEST_HPP
#define ENDERMAN_REQUEST_HPP

#include "types.hpp"
#include "constants.hpp"
#include "body.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace enderman
{
    class Request
    {
    private:
        HttpMethod _method;

        std::string _raw_uri;
        std::string _base_path;
        std::string _relative_path;
        std::vector<std::string> _base_path_segments;
        std::vector<std::string> _relative_path_segments;
        std::unordered_map<std::string, std::string> _path_params;
        std::unordered_map<std::string, std::string> _query_params;

        const std::unordered_map<std::string, std::string> _headers;

        std::shared_ptr<Body> body;

    public:
        Request(const HttpMethod method,
                const std::string &raw_uri,
                const std::unordered_map<std::string, std::string> &headers)
            : _method(method),
              _headers(headers),
              _raw_uri(raw_uri) {}

        ~Request() = default;

        HttpMethod method() const { return _method; }
        const std::string &raw_uri() const { return _raw_uri; }
        const std::string &base_path() const { return _base_path; }
        const std::string &relative_path() const { return _relative_path; }
        const std::vector<std::string> &base_path_segments() const { return _base_path_segments; }
        const std::vector<std::string> &relative_path_segments() const { return _relative_path_segments; }
        const std::unordered_map<std::string, std::string> &path_params() const { return _path_params; }
        const std::unordered_map<std::string, std::string> &query_params() const { return _query_params; }
        const std::unordered_map<std::string, std::string> &headers() const { return _headers; }
        std::shared_ptr<Body> get_body() const { return body; }
        void set_body(std::shared_ptr<Body> body_ptr) { body = body_ptr; }
        bool has_body() const { return body != nullptr; }

        friend class RequestBuilder;
    };
}

#endif // ENDERMAN_REQUEST_HPP