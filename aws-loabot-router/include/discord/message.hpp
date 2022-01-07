#pragma once

#include <string>

#include <aws/core/utils/json/JsonSerializer.h>

namespace discord {

enum class RequestType {
    Ping = 1,
    Command = 2
};

class ResponseBuilder
{
public:
    static Aws::Utils::Json::JsonValue Ping();
    static Aws::Utils::Json::JsonValue Pending();
};

}