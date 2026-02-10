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
            class InvalidURLEncodingException : public std::runtime_error
            {
            public:
                explicit InvalidURLEncodingException(const std::string &message)
                    : std::runtime_error(message) {}
            };

        public:
            class InvalidURIException : public std::runtime_error
            {
            public:
                explicit InvalidURIException(const std::string &message)
                    : std::runtime_error(message) {}
            };

            /// @brief Struct representing a parsed URI.
            /// @param path_segments Vector of path segments
            /// @param query_params Unordered map of query parameters
            struct ParsedURI
            {
                std::vector<std::string> path_segments;
                std::unordered_map<std::string, std::string> query_params;
            };

            /// @brief Parse a URI into URL decoded and normalized path segments and query parameters.
            /// @param uri URI string to parse
            /// @return ParsedURI struct containing path segments and query parameters. All URL decoded.
            /// @throws InvalidURIException if the URI is malformed or contains invalid characters.
            static ParsedURI parse_uri(const std::string &uri);
            /// @brief Parse a path string into segments. This function is meant to be used on URL decoded paths.
            /// @param path Path string to parse
            /// @return Vector of path segments
            static std::vector<std::string> parse_path(const std::string &path);
        };

        class PathTools
        {
        public:
            /// @brief Create a path string from a vector of segments. Linux style paths with '/' as separator.
            /// @param segments Vector of path segments
            /// @return Path string
            static std::string build_path(const std::vector<std::string> &segments);
            /// @brief Get path relative to a pattern. For example, if the path is /users/123/profile and the pattern is /users/:id, this function will return ["profile"].
            /// @param path_segments Vector of original path segments
            /// @param pattern_segments Vector of pattern segments.
            /// @return Vector of path segments relative to the pattern.
            /// @warning This function does not check if the pattern matches the path.
            /// It's the caller's responsibility to ensure that the pattern matches the path before calling this function.
            static std::vector<std::string> get_relative_path(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments);
            /// @brief Match the given path with a pattern.
            /// @param path_segments Vector of path segments
            /// @param pattern_segments Vector of pattern segments
            /// @return True if the path matches the pattern, false otherwise
            static bool match_full_path(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments);
            /// @brief Check if a given pattern is a prefix of the path.
            /// @param path_segments Vector of path segments
            /// @param pattern_segments Vector of pattern segments
            /// @return True if the pattern is a prefix of the path, false otherwise
            static bool match_prefix(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments);
            /// @brief Extract path parameters from a path based on a pattern.
            /// @param path_segments Vector of path segments
            /// @param pattern_segments Vector of pattern segments
            /// @return Unordered map of parameter names and their corresponding values extracted from the path.
            /// @warning This function only matches the path and pattern segment by segment.
            /// It does not checks if the pattern matches the path or is a prefix of the path.
            static std::unordered_map<std::string, std::string> extract_path_params(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments);
        };
    }
}

#endif // ENDERMAN_UTILS_HPP