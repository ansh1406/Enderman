#include "enderman/response.hpp"
#include "enderman/response_writer.hpp"
#include "enderman/body.hpp"

struct enderman::Response::Impl
{
    int status_code;
    std::string message;
    std::unordered_map<std::string, std::string> headers;
    Body *body;

    bool is_final;

    Impl() : status_code(200), body(nullptr), is_final(false) {}
    ~Impl()
    {
        delete body;
    }
};

int enderman::ResponseWriter::get_status_code(const enderman::Response &response)
{
    return response.pImpl->status_code;
}

std::string enderman::ResponseWriter::get_reason_phrase(const enderman::Response &response)
{
    return response.pImpl->message;
}

std::unordered_map<std::string, std::string> enderman::ResponseWriter::get_headers(const enderman::Response &response)
{
    return response.pImpl->headers;
}

std::vector<char> enderman::ResponseWriter::get_body(const enderman::Response &response)
{
    if (response.pImpl->body)
    {
        return response.pImpl->body->serialize();
    }
    return std::vector<char>();
}

enderman::Response::Response() : pImpl(new Impl()) {}

enderman::Response::~Response()
{
    delete pImpl;
}
enderman::Response &enderman::Response::set_status(int status_code)
{
    if (pImpl->is_final)
        return *this;
    pImpl->status_code = status_code;
    return *this;
}

enderman::Response &enderman::Response::set_message(const std::string &message)
{
    if (pImpl->is_final)
        return *this;
    pImpl->message = message;
    return *this;
}

enderman::Response &enderman::Response::set_header(const std::string &key, const std::string &value)
{
    if (pImpl->is_final)
        return *this;
    pImpl->headers[key] = value;
    return *this;
}

enderman::Response &enderman::Response::set_body(Body *body)
{
    if (pImpl->is_final)
        return *this;
    delete pImpl->body;
    pImpl->body = body;
    if (body)
        set_header("Content-Type", body->type());
    return *this;
}

void enderman::Response::send()
{
    pImpl->is_final = true;
}