///  @file formdata_body.hpp
///  @brief Derived class for URL-encoded form data type bodies and middleware to parse them.

#pragma once

#include "enderman/request.hpp"
#include "enderman/response.hpp"
#include "enderman/types.hpp"
#include "enderman/body.hpp"

#include <string>
#include <memory>
#include <vector>
#include <iomanip>
#include <sstream>
#include <unordered_map>

namespace enderman
{
    /// @brief Class representing a URL-encoded form data body in a request or response.
    class UrlEncodedFormDataBody : public Body
    {
    public:
        std::unordered_map<std::string, std::string> fields;

        UrlEncodedFormDataBody() = default;

        void parse_from(const std::vector<char> &body)
        {
            std::string str(body.begin(), body.end());
            std::istringstream ss(str);
            std::string pair;
            while (std::getline(ss, pair, '&'))
            {
                if (pair.empty())
                    continue;
                size_t pos = pair.find('=');
                if (pos != std::string::npos)
                {
                    std::string key = url_decode(pair.substr(0, pos));
                    std::string value = url_decode(pair.substr(pos + 1));
                    fields[key] = value;
                }
                else
                {
                    std::string key = url_decode(pair);
                    fields[key] = "";
                }
            }
        }

        std::vector<char> serialize() const override
        {
            std::ostringstream url_encoded_string;
            for (const auto &field : fields)
            {
                if (!url_encoded_string.str().empty())
                {
                    url_encoded_string << "&";
                }
                url_encoded_string << url_encode(field.first) << "=" << url_encode(field.second);
            }
            return std::vector<char>(url_encoded_string.str().begin(), url_encoded_string.str().end());
        }

        const std::string type() const override
        {
            return "application/x-www-form-urlencoded";
        }

        static std::string url_encode(const std::string &value)
        {
            std::ostringstream escaped;
            escaped.fill('0');
            escaped << std::hex;

            for (char c : value)
            {
                if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~')
                {
                    escaped << c;
                }
                else
                {
                    escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
                }
            }

            return escaped.str();
        }

        static std::string url_decode(const std::string &value)
        {
            std::string result;
            for (size_t i = 0; i < value.length(); ++i)
            {
                if (value[i] == '%' && i + 2 < value.length())
                {
                    std::string hex = value.substr(i + 1, 2);
                    char decoded_char = static_cast<char>(std::stoi(hex, nullptr, 16));
                    result.push_back(decoded_char);
                    i += 2;
                }
                else if (value[i] == '+')
                {
                    result.push_back(' ');
                }
                else
                {
                    result.push_back(value[i]);
                }
            }
            return result;
        }
    };

    /// @brief Middleware function to parse URL-encoded form data request bodies. It checks the Content-Type header of the request and if it is "application/x-www-form-urlencoded", it parses the body as a UrlEncodedFormDataBody and replaces the original RawBody with the parsed UrlEncodedFormDataBody in the request.
    MiddlewareFunction url_encoded_formdata_parser = MiddlewareFunction(
        [](Request &req, Response &res, Next next)
        {
            auto content_type_it = req.headers().find("content-type");
            if (content_type_it != req.headers().end())
            {
                std::string content_type = content_type_it->second;
                content_type = content_type_it->second;
                auto sem = content_type.find(';');
                if (sem != std::string::npos)
                    content_type = content_type.substr(0, sem);
                for (auto &c : content_type)
                    c = std::tolower(c);
                if (content_type == "application/x-www-form-urlencoded")
                {
                    std::shared_ptr<UrlEncodedFormDataBody> formdata_body = std::make_shared<UrlEncodedFormDataBody>();
                    formdata_body->parse_from(req.get_body()->as<RawBody>()->data);
                    req.set_body(formdata_body);
                }
            }
            next(nullptr);
        });
}