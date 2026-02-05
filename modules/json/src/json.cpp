#include "json/json.hpp"
#include "json_internal.hpp"
#include "nlohmann/json.hpp"

#include <memory>
#include <stdexcept>

namespace enderman_json
{

    struct Object::Impl
    {
        std::shared_ptr<ObjectInternal> internal;
        Impl() = default;
    };

    Object::Object()
    {
        pImpl = new Impl();
        pImpl->internal = std::make_shared<NullInternal>();
    }

    Object::Object(long long value)
    {
        pImpl = new Impl();
        pImpl->internal = std::make_shared<IntegerInternal>(value);
    }

    Object::Object(double value)
    {
        pImpl = new Impl();
        pImpl->internal = std::make_shared<DoubleInternal>(value);
    }

    Object::Object(const std::string &value)
    {
        pImpl = new Impl();
        pImpl->internal = std::make_shared<StringInternal>(value);
    }

    Object::Object(const char *value) : Object(std::string(value)) {}

    Object::Object(bool value)
    {
        pImpl = new Impl();
        pImpl->internal = std::make_shared<BooleanInternal>(value);
    }

    Object::Object(std::nullptr_t value) : Object() {}

    Object::Object(const json_array &value)
    {
        pImpl = new Impl();
        pImpl->internal = std::make_shared<ArrayInternal>(value);
    }

    Object::Object(const json_map &value)
    {
        pImpl = new Impl();
        pImpl->internal = std::make_shared<MapInternal>(value);
    }

    Object::~Object()
    {
        delete pImpl;
    }

    Object::Object(const Object &other) : pImpl(new Impl())
    {
        pImpl->internal = other.pImpl->internal;
    }

    Object &Object::operator=(const Object &other)
    {
        if (this != &other)
        {
            pImpl->internal = other.pImpl->internal;
        }
        return *this;
    }

    Object::Object(Object &&other) noexcept
    {
        this->pImpl->internal = std::move(other.pImpl->internal);
    }

    Object &Object::operator=(Object &&other) noexcept
    {
        if (this != &other)
        {
            this->pImpl->internal = std::move(other.pImpl->internal);
        }
        return *this;
    }

    int Object::as_int() const
    {
        if (this->type() != INTEGER)
        {
            throw std::bad_cast{};
        }
        return static_cast<IntegerInternal *>(this->pImpl->internal.get())->value;
    }

    long long Object::as_long_long() const
    {
        if (this->type() != INTEGER)
        {
            throw std::bad_cast{};
        }
        return static_cast<IntegerInternal *>(this->pImpl->internal.get())->value;
    }

    double Object::as_double() const
    {
        if (this->type() != DOUBLE)
        {
            throw std::bad_cast{};
        }
        return static_cast<DoubleInternal *>(this->pImpl->internal.get())->value;
    }

    std::string &Object::as_string() const
    {
        if (this->type() != STRING)
        {
            throw std::bad_cast{};
        }
        return static_cast<StringInternal *>(this->pImpl->internal.get())->value;
    }

    bool Object::as_bool() const
    {
        if (this->type() != BOOLEAN)
        {
            throw std::bad_cast{};
        }
        return static_cast<BooleanInternal *>(this->pImpl->internal.get())->value;
    }

    json_array &Object::as_array() const
    {
        if (this->type() != ARRAY)
        {
            throw std::bad_cast{};
        }
        return static_cast<ArrayInternal *>(this->pImpl->internal.get())->values;
    }

    json_map &Object::as_map() const
    {
        if (this->type() != MAP)
        {
            throw std::bad_cast{};
        }
        return static_cast<MapInternal *>(this->pImpl->internal.get())->values;
    }

    Type Object::type() const noexcept
    {
        return pImpl->internal->type();
    }

    Object Object::clone() const
    {
        if (this->type() == MAP)
        {
            json_map new_map;
            for (const auto &pair : this->as_map())
            {
                new_map[pair.first] = pair.second.clone();
            }
            return Object(new_map);
        }
        else if (this->type() == ARRAY)
        {
            json_array new_array;
            for (const auto &item : this->as_array())
            {
                new_array.push_back(item.clone());
            }
            return Object(new_array);
        }
        else if (this->type() == STRING)
        {
            return Object(this->as_string());
        }
        else if (this->type() == INTEGER)
        {
            return Object(this->as_long_long());
        }
        else if (this->type() == DOUBLE)
        {
            return Object(this->as_double());
        }
        else if (this->type() == BOOLEAN)
        {
            return Object(this->as_bool());
        }
        else // NULLTYPE
        {
            return Object();
        }
    }

    void Object::push_back(const Object &item)
    {
        if (this->type() != ARRAY)
        {
            throw std::bad_cast{};
        }
        static_cast<ArrayInternal *>(this->pImpl->internal.get())->values.push_back(item);
    }

    void Object::pop_back()
    {
        if (this->type() != ARRAY)
        {
            throw std::bad_cast{};
        }
        static_cast<ArrayInternal *>(this->pImpl->internal.get())->values.pop_back();
    }

    void Object::insert(const Object &value, const size_t index)
    {
        if (this->type() != ARRAY)
        {
            throw std::bad_cast{};
        }
        auto &arr = static_cast<ArrayInternal *>(this->pImpl->internal.get())->values;
        if (index > arr.size())
        {
            throw std::out_of_range("Index out of range");
        }
        arr.insert(arr.begin() + index, value);
    }

    void Object::erase(const size_t index)
    {
        if (this->type() != ARRAY)
        {
            throw std::bad_cast{};
        }
        auto &arr = static_cast<ArrayInternal *>(this->pImpl->internal.get())->values;
        if (index >= arr.size())
        {
            throw std::out_of_range("Index out of range");
        }
        arr.erase(arr.begin() + index);
    }

    void Object::find(const std::string &key)
    {
        if (this->type() != MAP)
        {
            throw std::bad_cast{};
        }
        auto &m = static_cast<MapInternal *>(this->pImpl->internal.get())->values;
        auto it = m.find(key);
        if (it == m.end())
        {
            throw std::out_of_range("Key not found");
        }
    }

    size_t Object::size() const
    {
        if (this->type() == ARRAY)
        {
            return static_cast<ArrayInternal *>(this->pImpl->internal.get())->values.size();
        }
        else if (this->type() == MAP)
        {
            return static_cast<MapInternal *>(this->pImpl->internal.get())->values.size();
        }
        else if (this->type() == STRING)
        {
            return static_cast<StringInternal *>(this->pImpl->internal.get())->value.size();
        }
        else
        {
            throw std::bad_cast{};
        }
    }

    Object &Object::operator[](const size_t index)
    {
        if (this->type() != ARRAY)
        {
            throw std::bad_cast{};
        }
        auto &arr = static_cast<ArrayInternal *>(this->pImpl->internal.get())->values;
        if (index >= arr.size())
        {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

    Object &Object::operator[](const size_t index) const
    {
        if (this->type() != ARRAY)
        {
            throw std::bad_cast{};
        }
        auto &arr = static_cast<ArrayInternal *>(this->pImpl->internal.get())->values;
        if (index >= arr.size())
        {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

    Object &Object::operator[](const std::string &key)
    {
        if (this->type() != MAP)
        {
            throw std::bad_cast{};
        }
        auto &m = static_cast<MapInternal *>(this->pImpl->internal.get())->values;
        auto it = m.find(key);
        if (it == m.end())
        {
            m[key] = Object();
        }
        return m[key];
    }

    Object &Object::operator[](const std::string &key) const
    {
        if (this->type() != MAP)
        {
            throw std::bad_cast{};
        }
        auto &m = static_cast<MapInternal *>(this->pImpl->internal.get())->values;
        auto it = m.find(key);
        if (it == m.end())
        {
            throw std::out_of_range("Key not found");
        }
        return m.at(key);
    }

    Object Object::operator+(const Object &other) const
    {
        if (this->type() == MAP && other.type() == MAP)
        {
            json_map new_map = this->as_map();
            for (const auto &pair : other.as_map())
            {
                new_map[pair.first] = pair.second;
            }
            return Object(new_map);
        }
        else if (this->type() == ARRAY && other.type() == ARRAY)
        {
            json_array new_array = this->as_array();
            for (const auto &item : other.as_array())
            {
                new_array.push_back(item);
            }
            return Object(new_array);
        }
        else if (this->type() == STRING && other.type() == STRING)
        {
            return Object(this->as_string() + other.as_string());
        }
        else if (this->type() == INTEGER && other.type() == INTEGER)
        {
            return Object(this->as_long_long() + other.as_long_long());
        }
        else if (this->type() == DOUBLE && other.type() == DOUBLE)
        {
            return Object(this->as_double() + other.as_double());
        }
        else if (this->type() == BOOLEAN && other.type() == BOOLEAN)
        {
            return Object(this->as_bool() || other.as_bool());
        }
        else if (this->type() == NULLTYPE && other.type() == NULLTYPE)
        {
            return Object();
        }
        else
        {
            throw std::invalid_argument("Both objects must be of the same type");
        }
    }

    Object parse_json(const nlohmann::json &nlohmann_json_obj)
    {
        switch (nlohmann_json_obj.type())
        {
        case nlohmann::json::value_t::null:
            return Object();
        case nlohmann::json::value_t::boolean:
            return Object(nlohmann_json_obj.get<bool>());
        case nlohmann::json::value_t::number_integer:
            return Object(nlohmann_json_obj.get<long long>());
        case nlohmann::json::value_t::number_float:
            return Object(nlohmann_json_obj.get<double>());
        case nlohmann::json::value_t::string:
            return Object(nlohmann_json_obj.get<std::string>());
        case nlohmann::json::value_t::array:
        {
            json_array arr;
            for (const auto &item : nlohmann_json_obj)
            {
                arr.push_back(parse_json(item));
            }
            return Object(arr);
        }
        case nlohmann::json::value_t::object:
        {
            json_map obj;
            for (auto it = nlohmann_json_obj.begin(); it != nlohmann_json_obj.end(); ++it)
            {
                obj[it.key()] = parse_json(it.value());
            }
            return Object(obj);
        }
        default:
            throw std::runtime_error("Unsupported JSON value type");
        }
    }

    Object parse_json(const std::string &json_str)
    {
        try
        {
            nlohmann::json nlohmann_json_obj = nlohmann::json::parse(json_str);
            return parse_json(nlohmann_json_obj);
        }
        catch (const nlohmann::json::parse_error &e)
        {
            throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
        }
    }

    std::string stringify(const Object &obj)
    {
        nlohmann::json nlohmann_json_obj;

        switch (obj.type())
        {
        case NULLTYPE:
            nlohmann_json_obj = nullptr;
            break;
        case BOOLEAN:
            nlohmann_json_obj = obj.as_bool();
            break;
        case INTEGER:
            nlohmann_json_obj = obj.as_long_long();
            break;
        case DOUBLE:
            nlohmann_json_obj = obj.as_double();
            break;
        case STRING:
            nlohmann_json_obj = obj.as_string();
            break;
        case ARRAY:
        {
            nlohmann::json arr = nlohmann::json::array();
            for (const auto &item : obj.as_array())
            {
                arr.push_back(nlohmann::json::parse(stringify(item)));
            }
            nlohmann_json_obj = arr;
            break;
        }
        case MAP:
        {
            nlohmann::json map = nlohmann::json::object();
            for (const auto &pair : obj.as_map())
            {
                map[pair.first] = nlohmann::json::parse(stringify(pair.second));
            }
            nlohmann_json_obj = map;
            break;
        }
        default:
            throw std::runtime_error("Unsupported JSON value type");
        }

        return nlohmann_json_obj.dump();
    }
}