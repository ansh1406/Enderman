/// @file body.hpp
/// @brief Defines the abstract Body class and a concrete RawBody class for handling message bodies in the Enderman library.
/// @brief You may make your own Body class by inheriting from Body and implementing the serialize() and type() methods.

#ifndef ENDERMAN_BODY_HPP
#define ENDERMAN_BODY_HPP

#include <vector>
#include <string>

namespace enderman
{
    /// @brief Abstract base class representing the body of a Request or Response.
    class Body
    {
    public:
        Body() = default;
        virtual ~Body() = default;

        /// @brief Serializes the body into a vector of characters.
        /// @return The serialized body as a vector of characters.
        virtual std::vector<char> serialize() const = 0;
        /// @brief Content type of the body, e.g. "application/json", "text/plain", etc.
        /// This will be used to set the Content-Type header of the message.
        /// @return Content type as a string.
        virtual const std::string type() const = 0;

        /// @brief Casts body to a specific type T.
        /// @tparam T Type to cast to, must be derived from Body.
        /// @return Object of type T if the cast is successful.
        /// @throws std::bad_cast if the cast fails.
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
        /// @brief Casts body to a specific type T.
        /// @tparam T Type to cast to, must be derived from Body.
        /// @return Object of type T if the cast is successful.
        /// @throws std::bad_cast if the cast fails.
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

    /// @brief A simple implementation of Body that holds raw binary data as a vector of characters.
    class RawBody : public Body
    {
    public:
        /// @brief The raw binary data of the body.
        std::vector<char> data;

        RawBody() = default;
        ~RawBody() override = default;
        /// @brief Set data in body from a vector of characters.
        /// @param body The vector of characters to set as the body data.
        void parse_from(const std::vector<char> &body)
        {
            data = body;
        }
        /// @brief Serializes the raw body data into vector of characters.
        /// @return The raw body data as a vector of characters.
        std::vector<char> serialize() const override
        {
            return data;
        }
        /// @brief Get type of RawBody.
        /// @return application/octet-stream
        const std::string type() const override { return std::string("application/octet-stream"); }
    };
}

#endif // ENDERMAN_BODY_HPP