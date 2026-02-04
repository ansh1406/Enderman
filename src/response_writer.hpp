#ifndef ENDERMAN_RESPONSE_WRITER_HPP
#define ENDERMAN_RESPONSE_WRITER_HPP

#include "enderman/types.hpp"

#include <unordered_map>
#include <vector>
#include <string>

namespace enderman
{
    class ResponseWriter
    {
    public:
        static int get_status_code(const Response &response);
        static std::string get_reason_phrase(const Response &response);
        static std::unordered_map<std::string, std::string> get_headers(const Response &response);
        static std::vector<char> get_body(const Response &response);
    };
}

#endif // ENDERMAN_RESPONSE_WRITER_HPP
