#pragma once
#include "json.hpp"
#include "enderman/body.hpp"

namespace enderman
{
    namespace json
    {

        class JsonBody : public enderman::Body
        {
        private:
            enderman::json::Object *jsonobj;
            static constexpr const char *TYPE = "application/json";

        public:
            JsonBody(enderman::json::Object *obj = new Null()) : jsonobj(obj) {}
            JsonBody *clone() const override
            {
                JsonBody *new_body = new JsonBody(jsonobj->clone());
                return new_body;
            }
            void parse_from(const std::vector<char> &body) override
            {
                std::string str(body.begin(), body.end());
                jsonobj = enderman::json::parse_json(str);
            }
            std::vector<char> serialize() const override
            {
                std::string str = enderman::json::stringify(jsonobj);
                return std::vector<char>(str.begin(), str.end());
            }
            const std::string type() const override { return std::string(TYPE); }

            ~JsonBody() override
            {
                delete jsonobj;
            }
        };
    }
}