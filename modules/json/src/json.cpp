#include "includes/json.hpp"
#include "nlohmann/json.hpp"

namespace enderman
{
    namespace json
    {

        String::String(const std::string &v) : value(v) {}

        Integer::Integer(long long v) : value(v) {}
        Integer::Integer() : value(0) {}

        Double::Double(double v) : value(v) {}
        Double::Double() : value(0.0) {}

        Boolean::Boolean(bool v) : value(v) {}
        Boolean::Boolean() : value(false) {}

        Map::~Map()
        {
            for (auto &[key, value] : values)
            {
                delete value;
            }
        }

        Array::~Array()
        {
            for (auto &value : values)
            {
                delete value;
            }
        }

        Object *&Map::operator[](const std::string &key)
        {
            if (values.find(key) == values.end())
            {
                values[key] = nullptr;
            }
            return values[key];
        }

        Object *Map::operator[](const std::string &key) const
        {
            auto it = values.find(key);
            if (it == values.end())
            {
                return nullptr;
            }
            return it->second;
        }

        size_t Map::size() const noexcept
        {
            return values.size();
        }

        bool Map::contains(const std::string &key) const
        {
            return values.find(key) != values.end();
        }

        Object *Map::clone() const
        {
            Map *new_map = new Map();
            for (const auto &[key, value] : values)
            {
                (*new_map)[key] = value->clone();
            }
            return new_map;
        }

        Object *&Array::operator[](size_t index)
        {
            return values[index];
        }

        Object *Array::operator[](size_t index) const
        {
            return values[index];
        }

        void Array::push(Object *obj)
        {
            values.push_back(obj);
        }

        Object *Array::back()
        {
            return values.back();
        }

        void Array::insert(Object *obj, size_t index)
        {
            values.insert(values.begin() + index, obj);
        }

        size_t Array::size() const noexcept
        {
            return values.size();
        }

        Object *Array::clone() const
        {
            Array *new_array = new Array();
            for (const auto &value : values)
            {
                new_array->push(value->clone());
            }
            return new_array;
        }

        Object *parse_json(const nlohmann::json &j)
        {
            if (j.is_object())
            {
                Map &res = *new Map();
                for (auto &[key, value] : j.items())
                    res[key] = parse_json(value);
                return &res;
            }
            else if (j.is_array())
            {
                Array &res = *new Array();
                for (const auto &elem : j.get<nlohmann::json::array_t>())
                {
                    res.push(parse_json(elem));
                }
                return &res;
            }
            else if (j.is_string())
            {
                return new String(j.get<std::string>());
            }
            else if (j.is_number_integer())
            {
                return new Integer(j.get<long long>());
            }
            else if (j.is_number_float())
            {
                return new Double(j.get<double>());
            }
            else if (j.is_boolean())
            {
                return new Boolean(j.get<bool>());
            }
            else if (j.is_null())
            {
                return new Null();
            }
            return new Null();
        }

        Object *parse_json(const std::string &json_str)
        {
            try
            {
                nlohmann::json raw_object = nlohmann::json::parse(json_str);
                Object *result_object = parse_json(raw_object);
                return result_object;
            }
            catch (const nlohmann::json::parse_error &e)
            {
                return new Null();
            }
            return new Null();
        }

        std::string stringify(enderman::json::Object *obj)
        {
            nlohmann::json j;
            switch (obj->type())
            {
            case enderman::json::NULLTYPE:
                j = nullptr;
                break;
            case enderman::json::MAP:
            {
                nlohmann::json map_json = nlohmann::json::object();
                for (const auto &[key, value] : obj->asMap())
                {
                    map_json[key] = nlohmann::json::parse(stringify(value));
                }
                j = map_json;
                break;
            }
            case enderman::json::ARRAY:
            {
                nlohmann::json array_json = nlohmann::json::array();
                for (const auto &value : obj->asArray())
                {
                    array_json.push_back(nlohmann::json::parse(stringify(value)));
                }
                j = array_json;
                break;
            }
            case enderman::json::STRING:
                j = obj->asString();
                break;
            case enderman::json::INTEGER:
                j = obj->asInteger();
                break;
            case enderman::json::DOUBLE:
                j = obj->asDouble();
                break;
            case enderman::json::BOOLEAN:
                j = obj->asBoolean();
                break;
            }
            return j.dump();
        }

        Object *clone_object(Object *obj)
        {
            if (!obj)
                return nullptr;
            return obj->clone();
        }
    }
}