#ifndef ENDERMAN_REQUEST_HPP
#define ENDERMAN_REQUEST_HPP

#include "enderman/types.hpp"
#include "enderman/constants.hpp"
#include "enderman/body.hpp"

#include <string>
#include <unordered_map>
#include <vector>

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
        Body *body;

    public:
        Request(const HttpMethod method,
                const std::vector<std::string> &path_segments,
                const std::unordered_map<std::string, std::string> &query_params,
                const std::unordered_map<std::string, std::string> &headers,
                Body *body_ptr)
            : _method(method),
              _path_segments(path_segments),
              _query_params(query_params),
              _headers(headers),
              body(body_ptr) {}

        HttpMethod method() const { return _method; }
        const std::vector<std::string> &path_segments() const { return _path_segments; }
        const std::unordered_map<std::string, std::string> &path_params() const { return _path_params; }
        void set_path_param(const std::string &key, const std::string &value) { _path_params[key] = value; }
        const std::unordered_map<std::string, std::string> &query_params() const { return _query_params; }
        const std::unordered_map<std::string, std::string> &headers() const { return _headers; }
        Body *get_body() const { return body; }
        bool has_body() const { return body != nullptr; }

        void set_body(Body *body_ptr)
        {
            delete body;
            body = body_ptr;
        }
    };
}

#endif // ENDERMAN_REQUEST_HPP