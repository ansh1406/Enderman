/// @file response.hpp
/// @brief Defines the Response class for handling HTTP responses in the Enderman library.

#ifndef ENDERMAN_RESPONSE_HPP
#define ENDERMAN_RESPONSE_HPP

#include "types.hpp"

#include <string>
#include <unordered_map>
#include <memory>

namespace enderman
{
    /// @brief Response class represents an HTTP response that can be sent back to the client.
    class Response
    {
    private:
        struct Impl;
        struct Impl *pImpl;

    public:
        explicit Response();
        ~Response();

        /// @brief Sets the HTTP status code of the response.
        /// @param status_code Status code to be set for the response.
        /// @return Reference to the object it was called from.
        Response &set_status(int status_code);
        /// @brief Sets the HTTP status message of the response.
        /// @param message Status message to be set for the response.
        /// @return Reference to the object it was called from.
        Response &set_message(const std::string &message);
        /// @brief Sets a header key-value pair for the response.
        /// @param key Header key to be set for the response.
        /// @param value Header value to be set for the response.
        /// @return Reference to the object it was called from.
        Response &set_header(const std::string &key, const std::string &value);
        /// @brief  Sets the body of the response. The body should be a shared pointer to an object of a class that inherits from Body.
        /// @param body Shared pointer to the body object to be set for the response.
        /// @return reference to the object it was called from.
        Response &set_body(std::shared_ptr<Body> body);

        /// @brief Sends the response back to the client. Once this method is called, the response is freezed and cannot be modified further.
        void send();
        /// @brief Checks if the response has already been sent to the client.
        /// @return True if the response has been sent, false otherwise.
        bool is_sent() const;

        friend class ResponseWriter;
    };
}

#endif // ENDERMAN_RESPONSE_HPP