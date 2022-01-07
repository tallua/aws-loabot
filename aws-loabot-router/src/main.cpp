#include <aws/lambda-runtime/runtime.h>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include "discord/validate.hpp"
#include "discord/message.hpp"

#include "loabot/log.hpp"

using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;
using namespace loabot::log;

int main(int, char**)
{
    Aws::InitAPI(Aws::SDKOptions());

    run_handler([](const invocation_request& req) {
        LOG(req.payload);
        const auto payload = JsonValue(req.payload);

        if (!discord::validate_message(payload.View())) {
            LOG("Error: validation failed");
            return invocation_response::failure("Invalid Request", "400");
        }
        LOG("Success: validation success");

        const auto body = JsonValue(payload.View().GetString("body"));
        const auto message_type = body.View().GetInteger("type");
        LOG("Message Type: ", message_type);

        switch(discord::RequestType(message_type))
        {
        case discord::RequestType::Ping:
            return invocation_response::success(
                discord::ResponseBuilder::Ping().View().WriteCompact(), "application/json");
        case discord::RequestType::Command:
            return invocation_response::failure("Not Implemented", "500");
        default:
            return invocation_response::failure("Unknown Message Type", "404");
        }
    });

    return 0;
}