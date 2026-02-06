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
        virtual void parse_from(const std::vector<char> &body) = 0;
        virtual std::vector<char> serialize() const = 0;
        virtual const std::string type() const = 0;

        template <typename T>
        T *as() const
        {
            if (this->type() == T::TYPE)
            {
                return static_cast<T *>(this);
            }
            throw std::bad_cast();
        }
    };

    class RawBody : public Body
    {
    public:
        static constexpr const char *TYPE = "application/octet-stream";
        std::vector<char> data;
        RawBody() = default;
        ~RawBody() override = default;
        void parse_from(const std::vector<char> &body) override
        {
            data = body;
        }
        std::vector<char> serialize() const override
        {
            return data;
        }
        const std::string type() const override { return std::string(TYPE); }
    };
}

#endif // ENDERMAN_BODY_HPP