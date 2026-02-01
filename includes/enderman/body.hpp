#ifndef ENDERMAN_BODY_HPP
#define ENDERMAN_BODY_HPP

#include <vector>
#include <string>

namespace enderman
{
    class Body
    {
    public:
        Body() = default;
        virtual void parse_from(const std::vector<char> &body) = 0;
        virtual std::vector<char> serialize() const = 0;
        virtual const std::string &type() const = 0;
    };

    class RawBody : public Body
    {
    private:
        std::vector<char> data;
        static constexpr const char *TYPE = "raw";

    public:
        RawBody() = default;
        void parse_from(const std::vector<char> &body) override
        {
            data = body;
        }
        std::vector<char> serialize() const override
        {
            return data;
        }
        const std::string &type() const override { return std::string(TYPE); }
    };

    template <typename T>
    T &get_body_as(Body &body)
    {
        if (body.type() == T::TYPE)
        {
            return static_cast<T &>(body);
        }
        throw std::bad_cast();
    }
}

#endif // ENDERMAN_BODY_HPP