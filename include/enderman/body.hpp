/// @file body.hpp
/// @brief Defines the abstract Body class and a concrete RawBody class for handling message bodies in the Enderman library.
/// @brief You may make your own Body class by inheriting from Body and implementing the serialize() and type() methods.

#ifndef ENDERMAN_BODY_HPP
#define ENDERMAN_BODY_HPP

#include <vector>
#include <string>

namespace enderman
{
    class Body
    {
    public:
        Body() = default;
        virtual ~Body() = default;
        virtual std::vector<char> serialize() const = 0;
        virtual const std::string type() const = 0;

        template <typename T>
        const T *as() const
        {
            auto casted = dynamic_cast<const T *>(this);
            if (!casted)
            {
                throw std::bad_cast();
            }
            return casted;
        }

        template <typename T>
        T *as()
        {
            auto casted = dynamic_cast<T *>(this);
            if (!casted)
            {
                throw std::bad_cast();
            }
            return casted;
        }
    };

    class RawBody : public Body
    {
    public:
        std::vector<char> data;
        RawBody() = default;
        ~RawBody() override = default;
        void parse_from(const std::vector<char> &body)
        {
            data = body;
        }
        std::vector<char> serialize() const override
        {
            return data;
        }
        const std::string type() const override { return std::string("application/octet-stream"); }
    };
}

#endif // ENDERMAN_BODY_HPP