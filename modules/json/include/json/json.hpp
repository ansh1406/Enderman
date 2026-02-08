/*
    Enderman Json - A simple JSON library for C++ based on nlohmann/json.
    This projects is created for Enderman framework to handle JSON data in requests and responses.
    Despite that, this is completely independent of Enderman and can be used in any C++ project.
*/

/// @file json.hpp
/// @brief Header file for the Enderman Json library. This file defines the Object class which represents a JSON value.

#ifndef ENDERMAN_JSON_HPP
#define ENDERMAN_JSON_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

/// @brief All the functions and classes of the Enderman Json library are defined in this namespace.
namespace enderman_json
{
    /// @brief Type aliases for vector of JSON objects.
    using json_array = std::vector<class Object>;
    /// @brief Type aliases for map of JSON objects.
    using json_map = std::unordered_map<std::string, class Object>;

    /// @brief Enum for JSON value types.
    enum Type
    {
        NULLTYPE,
        MAP,
        ARRAY,
        STRING,
        INTEGER,
        DOUBLE,
        BOOLEAN,
    };

    /// @brief Object class represents a JSON value which can be of type null, boolean, integer, double, string, array or map.
    class Object
    {
    private:
        struct Impl;
        struct Impl *pImpl = nullptr;

    public:
        /// @brief Default constructor for Object class which creates a null JSON value.
        Object();
        /// @brief Constructors for Integer type JSON value.
        /// @param value Integer value to be stored in the JSON object.
        Object(long long value);
        /// @brief Constructors for Double type JSON value.
        /// @param value Double value to be stored in the JSON object.
        Object(double value);
        /// @brief Constructors for String type JSON value.
        /// @param value String value to be stored in the JSON object.
        Object(const std::string &value);
        /// @brief Constructors for String type JSON value.
        /// @param value C-string value to be stored in the JSON object.
        Object(const char *value);
        /// @brief Constructors for Boolean type JSON value.
        /// @param value Boolean value to be stored in the JSON object.
        Object(bool value);
        /// @brief Constructors for Null type JSON value.
        /// @param value nullptr to create a null JSON value.
        Object(std::nullptr_t value);
        /// @brief Constructors for Array type JSON value.
        /// @param value Vector of JSON objects to be stored in the JSON object.
        Object(const json_array &value);
        /// @brief Constructors for Map type JSON value.
        /// @param value Unordered map of string keys and JSON object values to be stored in the JSON object.
        Object(const json_map &value);
        /// @brief Copy constructor for Object class. This only creates a refenrence to the same underlying JSON value. Use clone() method to create a deep copy.
        /// @param other Another Object instance to copy from.
        Object(const Object &other);
        /// @brief Move constructor for Object class. This creates a refenrence to the value of the other Object and sets the other Object to null.
        Object(Object &&other) noexcept;
        ~Object();

        /// @brief Get the type of the JSON value stored in the Object.
        /// @return Type of the JSON value as a Type enum value.
        Type type() const noexcept;
        /// @brief Create a deep copy of the Object.
        /// @return A new Object instance that is a deep copy of the current Object.
        Object clone() const;

        /// @brief Copy assignment operator for Object class. This only creates a refenrence to the same underlying JSON value. Use clone() method to create a deep copy.
        /// @param other Another Object instance to copy from.
        /// @return Reference to the current Object instance.
        Object &operator=(const Object &other);
        /// @brief Move assignment operator for Object class. This creates a refenrence to the value of the other Object and sets the other Object to null.
        /// @param other Another Object instance to move from.
        /// @return Reference to the current Object instance.
        Object &operator=(Object &&other) noexcept;

        /// @brief Get the JSON value as an integer.
        /// @return Integer value of the JSON object.
        int as_int() const;
        /// @brief Get the JSON value as a long long.
        /// @return Long long value of the JSON object.
        long long as_long_long() const;
        /// @brief Get the JSON value as a double.
        /// @return Double value of the JSON object.
        double as_double() const;
        /// @brief Get the JSON value as a string.
        /// @return String value of the JSON object.
        std::string &as_string() const;
        /// @brief Get the JSON value as a boolean.
        /// @return Boolean value of the JSON object.
        bool as_bool() const;
        /// @brief Get the JSON value as an array.
        /// @return Array value of the JSON object.
        json_array &as_array() const;
        /// @brief Get the JSON value as a map.
        /// @return Map value of the JSON object.
        json_map &as_map() const;

        /// @brief Add an item to the end of the JSON array.
        /// @param item JSON object to be added.
        void push_back(const Object &item);
        /// @brief Remove the last item from the JSON array.
        void pop_back();
        /// @brief Insert an item at the specified index in the JSON array.
        /// @param value JSON object to be inserted.
        /// @param index Index at which the item should be inserted.
        void insert(const Object &value, const size_t index);
        /// @brief Remove an item at the specified index in the JSON array.
        /// @param index Index of the item to be removed.
        void erase(const size_t index);
        /// @brief Check if a key exists in the JSON object.
        /// @param key Key to be checked.
        /// @return True if the key exists, false otherwise.
        bool find(const std::string &key) const;

        /// @brief Get the number of elements in the JSON array or object.
        /// @return Number of elements if Array or Map ,length of the string if String otherwise throws.
        size_t size() const;

        /// @brief Access an element in the JSON array by index.
        /// @param index Index of the element to access.
        /// @return Reference to the JSON object at the specified index.
        Object &operator[](const size_t index);
        /// @brief Access an element in the JSON array by index.
        /// @param index Index of the element to access.
        /// @return Reference to the JSON object at the specified index.
        Object &operator[](const size_t index) const;
        /// @brief Access an element in the JSON object by key.
        /// @param key Key of the element to access.
        /// @return Reference to the JSON object corresponding to the specified key.
        Object &operator[](const std::string &key);
        /// @brief Access an element in the JSON object by key.
        /// @param key Key of the element to access.
        /// @return Reference to the JSON object corresponding to the specified key.
        Object &operator[](const std::string &key) const;
        /// @brief Add two JSON objects. The behavior of this operator is defined as follows:
        /// - If both objects are of type MAP, they are merged. If there are duplicate keys, the values from the other object overwrite the values from the current object.
        /// - If both objects are of type ARRAY, they are concatenated.
        /// - If both objects are of the same primitive type (STRING, INTEGER, DOUBLE, BOOLEAN), the value from the other object is added to the value of the current object (string concatenation for STRING, addition for INTEGER and DOUBLE, logical OR for BOOLEAN).
        /// - If the objects are of different types or if they are of type NULLTYPE, an exception is thrown.
        /// @param other JSON object to be added.
        /// @return Resulting JSON object after addition.
        Object operator+(const Object &other) const;
    };

    /// @brief Parse a JSON string into a JSON object.
    /// @param json_str JSON string to be parsed.
    /// @return Parsed JSON object.
    Object parse_json(const std::string &json_str);
    /// @brief Convert a JSON object into a JSON string.
    /// @param obj JSON object to be converted.
    /// @return JSON string representation of the object.
    std::string stringify(const Object &obj);
}

#endif // ENDERMAN_JSON_HPP