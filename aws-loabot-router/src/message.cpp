#include "discord/message.hpp"

using namespace discord;
using namespace Aws::Utils::Json;


Response ResponseBuilder::Ping() {
    return {JsonValue(R"({
            "statusCode": 200,
            "isBase64Encoded": false,
            "headers": {
                "content-type": "application/json"
            },
            "body": "{\"type\": 1 }"
        })").View().WriteCompact()};
}

Response ResponseBuilder::Pending() {
    return {JsonValue(R"({
            "statusCode": 200,
            "isBase64Encoded": false,
            "headers": {
                "content-type": "application/json"
            },
            "body": "{\"type\": 5 }"
        })").View().WriteCompact()};
}

Response ResponseBuilder::Failed(const std::string& message) {
    auto response = JsonValue(R"({
        "statusCode": 200,
        "isBase64Encoded": false,
        "headers": {
            "content-type": "application/json"
        }
    })");

    response.WithString("body",
        JsonValue()
            .WithInteger("type", 4)
            .WithObject("data", JsonValue().WithString("content", message)).View().WriteCompact());
    return { response.View().WriteCompact() };
}