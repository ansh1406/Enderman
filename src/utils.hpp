#ifndef ENDERMAN_UTILS_HPP
#define ENDERMAN_UTILS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace enderman
{
    namespace utils
    {
        class UriParser
        {
        private:
            static std::string decode_url_encoding(const std::string &encoded);
            static std::string strip_uri_fragment(const std::string &uri);
            static std::string path_from_uri(const std::string &uri);
            static std::string query_from_uri(const std::string &uri);
            static std::vector<std::string> split_path(const std::string &path);
            static std::vector<std::string> normalize_path(const std::vector<std::string> &segments);
            static std::unordered_map<std::string, std::string> parse_query_params(const std::string &query);

            static bool is_valid_path(const std::vector<std::string> &segments);
            static bool is_valid_query(const std::unordered_map<std::string, std::string> &query);

        public:
            class InvalidURLEncodingException : public std::runtime_error
            {
            public:
                explicit InvalidURLEncodingException(const std::string &message)
                    : std::runtime_error(message) {}
            };

            class InvalidURIException : public std::runtime_error
            {
            public:
                explicit InvalidURIException(const std::string &message)
                    : std::runtime_error(message) {}
            };

            struct ParsedURI
            {
                std::vector<std::string> path_segments;
                std::unordered_map<std::string, std::string> query_params;
            };

            static ParsedURI parse_uri(const std::string &uri);
            static std::vector<std::string> parse_path(const std::string &path);
        };

        class PathTools
        {
        public:
            static std::string build_path(const std::vector<std::string> &segments);
            static std::vector<std::string> get_relative_path(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments);
            static bool match_full_path(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments);
            static bool match_prefix(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments);
            static std::unordered_map<std::string, std::string> extract_path_params(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments);
        };
    }
}

#endif // ENDERMAN_UTILS_HPP