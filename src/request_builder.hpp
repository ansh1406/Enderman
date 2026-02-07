#ifndef ENDERMAN_REQUEST_BUILDER_HPP
#define ENDERMAN_REQUEST_BUILDER_HPP

#include "enderman/request.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace enderman
{
    class RequestBuilder
    {
    public:
        static void set_base_path(Request &request, const std::string &base_path);
        static void set_base_path_segments(Request &request, const std::vector<std::string> &base_path_segments);
        static void set_relative_path(Request &request, const std::string &relative_path);
        static void set_relative_path_segments(Request &request, const std::vector<std::string> &relative_path_segments);
        static void set_path_params(Request &request, const std::unordered_map<std::string, std::string> &path_params);
        static void set_query_params(Request &request, const std::unordered_map<std::string, std::string> &query_params);
    };
}

#endif // ENDERMAN_REQUEST_BUILDER_HPP