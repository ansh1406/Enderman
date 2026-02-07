#include "enderman/request.hpp"
#include "request_builder.hpp"

void enderman::RequestBuilder::set_base_path(Request &request, const std::string &base_path)
{
    request._base_path = base_path;
}

void enderman::RequestBuilder::set_base_path_segments(Request &request, const std::vector<std::string> &base_path_segments)
{
    request._base_path_segments = base_path_segments;
}

void enderman::RequestBuilder::set_relative_path(Request &request, const std::string &relative_path)
{
    request._relative_path = relative_path;
}

void enderman::RequestBuilder::set_relative_path_segments(Request &request, const std::vector<std::string> &relative_path_segments)
{
    request._relative_path_segments = relative_path_segments;
}

void enderman::RequestBuilder::set_path_params(Request &request, const std::unordered_map<std::string, std::string> &path_params)
{
    request._path_params = path_params;
}

void enderman::RequestBuilder::set_query_params(Request &request, const std::unordered_map<std::string, std::string> &query_params)
{
    request._query_params = query_params;
}