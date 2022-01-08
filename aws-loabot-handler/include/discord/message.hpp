#pragma once

#include <string>

namespace discord {

enum class RequestType {
    Ping = 1,
    Command = 2
};

struct Response {
    std::string message;

    const std::string& to_string() const { return message; }
};

class ResponseBuilder
{
public:
    static Response Ping();
    static Response Pending();
    static Response Failed(const std::string& message = "");
    static Response Message(const std::string& message = "");
};

}