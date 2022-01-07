#include "discord/message.hpp"

using namespace discord;
using namespace Aws::Utils::Json;

JsonValue ResponseBuilder::Ping()
{
    return JsonValue(R"({
        "statusCode": 200,
        "isBase64Encoded": false,
        "headers": {
            "content-type": "application/json"
        },
        "body": "{\"type\": 1 }"
    })");
}

JsonValue ResponseBuilder::Pending()
{
    return JsonValue(R"({
        "statusCode": 200,
        "isBase64Encoded": false,
        "headers": {
            "content-type": "application/json"
        },
        "body": "{\"type\": 5 }"
    })");
}