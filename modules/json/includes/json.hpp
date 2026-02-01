#ifndef ENDERMAN_JSON_HPP
#define ENDERMAN_JSON_HPP

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

namespace enderman
{
    namespace json
    {
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
        public:
            Object() = default;
            virtual ~Object() = default;
            virtual int type() const noexcept = 0;
            virtual std::map<std::string, Object *> &asMap() { throw std::bad_cast{}; }
            virtual std::vector<Object *> &asArray() { throw std::bad_cast{}; }
            virtual std::string &asString() { throw std::bad_cast{}; }
            virtual long long &asInteger() { throw std::bad_cast{}; }
            virtual double &asDouble() { throw std::bad_cast{}; }
            virtual bool &asBoolean() { throw std::bad_cast{}; }
        };

        class Null : public Object
        {
        public:
            Null() = default;
            int type() const noexcept override { return NULLTYPE; }
        };

        class Map : public Object
        {
        private:
            std::map<std::string, Object *> values;

        public:
            Map() = default;
            int type() const noexcept override { return MAP; }
            std::map<std::string, Object *> &asMap() override { return values; }
            Object *&operator[](const std::string &key);
            Object *operator[](const std::string &key) const;
            size_t size() const noexcept;
            bool contains(const std::string &key) const;
        };

        class String : public Object
        {
        private:
            std::string value;

        public:
            String(const std::string &v);
            String() = default;
            int type() const noexcept override { return STRING; }
            std::string &asString() override { return value; }
        };

        class Number : public Object
        {
        public:
            Number() = default;
            virtual ~Number() = default;
        };

        class Integer : public Number
        {
        private:
            long long value;

        public:
            Integer(long long v);
            Integer();
            int type() const noexcept override { return INTEGER; }
            long long &asInteger() override { return value; }
        };

        class Double : public Number
        {
        private:
            double value;

        public:
            Double(double v);
            Double();
            int type() const noexcept override { return DOUBLE; }
            double &asDouble() override { return value; }
        };

        class Boolean : public Number
        {
        private:
            bool value;

        public:
            Boolean(bool v);
            Boolean();
            int type() const noexcept override { return BOOLEAN; }
            bool &asBoolean() override { return value; }
        };

        class Array : public Object
        {
        private:
            std::vector<Object *> values;

        public:
            Array() = default;
            int type() const noexcept override { return ARRAY; }
            std::vector<Object *> &asArray() override { return values; }
            Object *&operator[](size_t index);
            Object *operator[](size_t index) const;
            void push(Object *obj);
            Object *back();
            void insert(Object *obj, size_t index);
            size_t size() const noexcept;
        };
        enderman::json::Object *parse_json(const std::string &json_str);
        std::string stringify(enderman::json::Object *obj);
        void free_json(enderman::json::Object *obj);
    }
}

#endif // ENDERMAN_JSON_HPP