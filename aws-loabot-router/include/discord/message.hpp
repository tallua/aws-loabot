#pragma once

#include <string>

#include <aws/core/utils/json/JsonSerializer.h>

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
};

}