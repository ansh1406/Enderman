/// @file constants.hpp
/// @brief Defines constants and enumerations used in the Enderman library.

#ifndef ENDERMAN_CONSTANTS_HPP
#define ENDERMAN_CONSTANTS_HPP

namespace enderman
{
    enum class HttpMethod
    {
        GET,
        POST,
        PUT,
        DELETE,
        PATCH,
        OPTIONS,
        HEAD,

    };
}

#endif // ENDERMAN_CONSTANTS_HPP