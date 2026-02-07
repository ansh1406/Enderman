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
        const HttpMethod _method;
        const std::vector<std::string> _path_segments;
        std::unordered_map<std::string, std::string> _path_params;
        const std::unordered_map<std::string, std::string> _query_params;
        const std::unordered_map<std::string, std::string> _headers;
        std::shared_ptr<Body> body;

    public:
        Request(const HttpMethod method,
                const std::vector<std::string> &path_segments,
                const std::unordered_map<std::string, std::string> &query_params,
                const std::unordered_map<std::string, std::string> &headers,
                std::shared_ptr<Body> body_ptr)
            : _method(method),
              _path_segments(path_segments),
              _query_params(query_params),
              _headers(headers),
              body(body_ptr) {}

        ~Request() = default;

        HttpMethod method() const { return _method; }
        const std::vector<std::string> &path_segments() const { return _path_segments; }
        const std::unordered_map<std::string, std::string> &path_params() const { return _path_params; }
        void set_path_param(const std::string &key, const std::string &value) { _path_params[key] = value; }
        const std::unordered_map<std::string, std::string> &query_params() const { return _query_params; }
        const std::unordered_map<std::string, std::string> &headers() const { return _headers; }
        std::shared_ptr<Body> get_body() const { return body; }
        void set_body(std::shared_ptr<Body> body_ptr) { body = body_ptr; }
        bool has_body() const { return body != nullptr; }
    };
}

#endif // ENDERMAN_REQUEST_HPP