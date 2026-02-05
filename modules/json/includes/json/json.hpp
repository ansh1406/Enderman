#ifndef ENDERMAN_JSON_HPP
#define ENDERMAN_JSON_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace enderman_json
{
    using json_array = std::vector<class Object>;
    using json_map = std::unordered_map<std::string, class Object>;

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

    class Object
    {
    private:
        struct Impl;
        struct Impl *pImpl = nullptr;

    public:
        Object();
        Object(long long value);
        Object(double value);
        Object(const std::string &value);
        Object(const char *value);
        Object(bool value);
        Object(std::nullptr_t value);
        Object(const json_array &value);
        Object(const json_map &value);
        Object(const Object &other);
        Object(Object &&other) noexcept;
        ~Object();

        Type type() const noexcept;
        Object clone() const;

        Object &operator=(const Object &other);
        Object &operator=(Object &&other) noexcept;

        int as_int() const;
        long long as_long_long() const;
        double as_double() const;
        std::string &as_string() const;
        bool as_bool() const;
        json_array &as_array() const;
        json_map &as_map() const;

        void push_back(const Object &item);
        void pop_back();
        void insert(const Object &value, const size_t index);
        void erase(const size_t index);
        void find(const std::string &key);

        size_t size() const;

        Object &operator[](const size_t index);
        Object &operator[](const size_t index) const;
        Object &operator[](const std::string &key);
        Object &operator[](const std::string &key) const;

        Object operator+(const Object &other) const;
    };

    Object parse_json(const std::string &json_str);
    std::string stringify(const Object &obj);
}

#endif // ENDERMAN_JSON_HPP