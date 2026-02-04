#include "src/utils.hpp"

#include <sstream>
#include <cctype>

enderman::utils::UriParser::ParsedURI enderman::utils::UriParser::parse_uri(const std::string &uri)
{
    try
    {
        std::string uri_no_fragment = strip_uri_fragment(uri);
        std::string path = path_from_uri(uri_no_fragment);
        std::string query = query_from_uri(uri_no_fragment);

        auto path_segments = split_path(path);
        auto normalized_segments = normalize_path(path_segments);
        auto query_params = parse_query_params(query);

        for (auto &segment : normalized_segments)
        {
            segment = decode_url_encoding(segment);
        }

        std::unordered_map<std::string, std::string> decoded_query_params;

        for (auto &pair : query_params)
        {
            decoded_query_params[decode_url_encoding(pair.first), decode_url_encoding(pair.second)];
        }

        if (!is_valid_path(normalized_segments))
        {
            throw InvalidURIException("Invalid path in URI: " + path);
        }

        if (!is_valid_query(decoded_query_params))
        {
            throw InvalidURIException("Invalid query in URI: " + query);
        }

        return ParsedURI{normalized_segments, decoded_query_params};
    }
    catch (const InvalidURLEncodingException &e)
    {
        throw InvalidURIException("Invalid URL encoding in URI: " + std::string(e.what()));
    }
    catch (const std::exception &e)
    {
        throw InvalidURIException("Failed to parse URI: " + std::string(e.what()));
    }
}

std::vector<std::string> enderman::utils::UriParser::parse_path(const std::string &path)
{
    auto segments = split_path(path);
    auto normalized_segments = normalize_path(segments);

    return normalized_segments;
}

std::string enderman::utils::UriParser::decode_url_encoding(const std::string &encoded)
{
    try
    {
        std::stringstream decoded;
        char ch;
        int i, ii;
        int len = encoded.length();
        for (i = 0; i < len; i++)
        {
            if (encoded[i] == '%')
            {
                decoded << static_cast<char>(std::stoi(encoded.substr(i + 1, 2), nullptr, 16));
                i = i + 2;
            }
            else if (encoded[i] == '+')
            {
                decoded << ' ';
            }
            else
            {
                decoded << encoded[i];
            }
        }
        return decoded.str();
    }
    catch (const std::exception &e)
    {
        throw InvalidURLEncodingException("Failed to decode URL encoding: " + std::string(e.what()));
    }
}

std::string enderman::utils::UriParser::strip_uri_fragment(const std::string &uri)
{
    auto pos = uri.find('#');
    if (pos == std::string::npos)
        return uri;
    return uri.substr(0, pos);
}

std::string enderman::utils::UriParser::path_from_uri(const std::string &uri)
{
    auto pos = uri.find('?');
    if (pos == std::string::npos)
        return uri;
    return uri.substr(0, pos);
}

std::string enderman::utils::UriParser::query_from_uri(const std::string &uri)
{
    auto pos = uri.find('?');
    if (pos == std::string::npos || pos + 1 >= uri.size())
        return "";
    return uri.substr(pos + 1);
}

std::vector<std::string> enderman::utils::UriParser::split_path(const std::string &path)
{
    std::vector<std::string> segments;
    size_t start = 0;
    size_t end = path.find('/');
    while (end != std::string::npos)
    {
        if (end != start)
            segments.push_back(path.substr(start, end - start));
        start = end + 1;
        end = path.find('/', start);
    }

    if (start < path.size())
        segments.push_back(path.substr(start));

    return segments;
}

std::vector<std::string> enderman::utils::UriParser::normalize_path(const std::vector<std::string> &segments)
{
    std::vector<std::string> normalized;
    for (const auto &segment : segments)
    {
        if (segment == "..")
        {
            if (!normalized.empty())
                normalized.pop_back();
        }
        else if (segment != "." && !segment.empty())
        {
            normalized.push_back(segment);
        }
    }
    return normalized;
}

std::unordered_map<std::string, std::string> enderman::utils::UriParser::parse_query_params(const std::string &query)
{
    std::unordered_map<std::string, std::string> params;
    size_t start = 0;
    while (start < query.size())
    {
        size_t end = query.find('&', start);
        if (end == std::string::npos)
            end = query.size();

        size_t eq_pos = query.find('=', start);
        if (eq_pos != std::string::npos && eq_pos < end)
        {
            std::string key = query.substr(start, eq_pos - start);
            std::string value = query.substr(eq_pos + 1, end - eq_pos - 1);
            params[key] = value;
        }
        else
        {
            std::string key = query.substr(start, end - start);
            params[key] = "";
        }

        start = end + 1;
    }
    return params;
}

bool enderman::utils::UriParser::is_valid_path(const std::vector<std::string> &segments)
{
    for (const auto &segment : segments)
    {
        for (unsigned char c : segment)
        {
            if (c == '\0' || std::iscntrl(c) || c == '\\')
                return false;
        }
    }
    return true;
}

bool enderman::utils::UriParser::is_valid_query(const std::unordered_map<std::string, std::string> &query)
{
    for (const auto &pair : query)
    {
        for (unsigned char c : pair.first)
        {
            if (c == '\0' || std::iscntrl(c))
                return false;
        }
        for (unsigned char c : pair.second)
        {
            if (c == '\0' || std::iscntrl(c))
                return false;
        }
    }
    return true;
}

bool enderman::utils::PathMatcher::match(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments)
{
    if (pattern_segments.size() != path_segments.size())
        return false;

    for (size_t i = 0; i < path_segments.size(); ++i)
    {
        if (pattern_segments[i].empty() || path_segments[i].empty())
            return false;
        if (pattern_segments[i] == "*")
            continue; // Wildcard segment

        if (pattern_segments[i][0] == ':')
            continue; // Parameter segment

        if (pattern_segments[i] != path_segments[i])
            return false;
    }

    return true;
}

std::unordered_map<std::string, std::string> enderman::utils::PathMatcher::extract_path_params(const std::vector<std::string> &path_segments, const std::vector<std::string> &pattern_segments)
{
    std::unordered_map<std::string, std::string> params;

    for (size_t i = 0; i < pattern_segments.size(); ++i)
    {
        if (pattern_segments[i].empty() || path_segments[i].empty())
            continue; // Skip invalid segments

        if (pattern_segments[i][0] == ':')
        {
            std::string param_name = pattern_segments[i].substr(1);
            params[param_name] = path_segments[i];
        }
    }

    return params;
}