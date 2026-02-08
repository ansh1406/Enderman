/// @file binary_body.hpp
/// @brief Derived class for binary type bodies.

#pragma once

#include "enderman/body.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <utility>

namespace enderman
{
    /// @brief BinaryBody class that inherits from Body and represents binary request or response bodies.
    class BinaryBody : public Body
    {
    public:
        std::vector<char> data;
        std::string content_type;
        BinaryBody() = default;

        void set_data(const std::vector<char> &_data)
        {
            data = _data;
        }

        void move_data(std::vector<char> &&_data)
        {
            data = std::move(_data);
        }

        void reset_data()
        {
            data.clear();
        }

        std::vector<char> serialize() const override
        {
            return data;
        }

        const std::string type() const override
        {
            return content_type;
        }
    };
}