#include <string>
#include <vector>
#include <unordered_map>

#include "enderman_json/json.hpp"

namespace enderman_json
{
    class ObjectInternal
    {
    public:
        ObjectInternal() = default;
        virtual ~ObjectInternal() = default;
        virtual Type type() const noexcept = 0;
    };

    class NullInternal : public ObjectInternal
    {
    public:
        NullInternal() = default;
        ~NullInternal() = default;
        Type type() const noexcept override { return NULLTYPE; }
    };

    class MapInternal : public ObjectInternal
    {

    public:
        std::unordered_map<std::string, Object> values;
        MapInternal() = default;
        MapInternal(const json_map &vals) : values(vals) {}
        ~MapInternal() = default;
        Type type() const noexcept override { return MAP; }
    };

    class ArrayInternal : public ObjectInternal
    {

    public:
        json_array values;
        ArrayInternal() = default;
        ArrayInternal(const json_array &vals) : values(vals) {}
        ~ArrayInternal() = default;
        Type type() const noexcept override { return ARRAY; }
    };

    class StringInternal : public ObjectInternal
    {

    public:
        std::string value;
        StringInternal(const std::string &v) : value(v) {}
        StringInternal() : value("") {}
        ~StringInternal() = default;
        Type type() const noexcept override { return STRING; }
    };

    class NumberInternal : public ObjectInternal
    {
    public:
        NumberInternal() = default;
        virtual ~NumberInternal() = default;
    };

    class IntegerInternal : public NumberInternal
    {
    public:
        long long value;
        IntegerInternal(long long v) : value(v) {}
        IntegerInternal() : value(0) {}
        Type type() const noexcept override { return INTEGER; }
    };

    class DoubleInternal : public NumberInternal
    {
    public:
        double value;
        DoubleInternal(double v) : value(v) {}
        DoubleInternal() : value(0.0) {}
        Type type() const noexcept override { return DOUBLE; }
    };

    class BooleanInternal : public NumberInternal
    {
    public:
        bool value;
        BooleanInternal(bool v) : value(v) {}
        BooleanInternal() : value(false) {}
        Type type() const noexcept override { return BOOLEAN; }
    };
}