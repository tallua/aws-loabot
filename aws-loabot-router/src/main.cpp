#include <aws/lambda-runtime/runtime.h>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include "discord/validate.hpp"
#include "discord/message.hpp"

#include "loabot/log.hpp"
#include "loabot/router.hpp"

using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;
using namespace loabot::log;
using namespace loabot::route;

int main(int, char**)
{
    Aws::InitAPI(Aws::SDKOptions());

    auto builder = LoabotRouterBuilder();
    auto router = std::shared_ptr<Router>(builder.build());

    run_handler([router](const invocation_request& req) {
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
        case discord::RequestType::Ping: {
            auto resp = discord::ResponseBuilder::Ping().to_string();
            LOG(resp);
            return invocation_response::success(resp, "application/json");
        }
        case discord::RequestType::Command: {
            auto resp = router->route(body, {
                payload.View().GetObject("stageVariables").GetString("DISCORD_APP_ID"),
                payload.View().GetObject("stageVariables").GetString("DISCORD_BOT_TOKEN"),
                payload.View().GetObject("stageVariables").GetString("DISCORD_ROUTE_TABLE_ARN")
            }).to_string();
            LOG(resp);
            return invocation_response::success(resp, "application/json");
        }
        default:
            LOG("Error: unknown message type");
            return invocation_response::failure("Unknown Message Type", "404");
        }
    });

    return 0;
}