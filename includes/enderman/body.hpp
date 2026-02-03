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
        virtual ~Body() = default;
        virtual Body *clone() const = 0;
        virtual void parse_from(const std::vector<char> &body) = 0;
        virtual std::vector<char> serialize() const = 0;
        virtual const std::string &type() const = 0;

        template <typename T>
        T *as() const
        {
            if (*this.type() == T::TYPE)
            {
                return static_cast<T *>(this);
            }
            throw std::bad_cast();
        }
    };

    class RawBody : public Body
    {
    private:
        std::vector<char> data;
        static constexpr const char *TYPE = "raw";

    public:
        RawBody() = default;
        ~RawBody() override = default;
        RawBody *clone() const override
        {
            RawBody *new_body = new RawBody();
            new_body->data = data;
            return new_body;
        }
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

}

#endif // ENDERMAN_BODY_HPP