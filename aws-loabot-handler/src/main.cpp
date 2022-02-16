#include <aws/lambda-runtime/runtime.h>

#include <sstream>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <optional>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/http/URI.h>

#include "discord/message.hpp"
#include "loabot/log.hpp"
#include "loabot/loabot_handler.hpp"
#include "loabot/loabot_http.hpp"

#include "discord/discord_message.hpp"
#include "discord/discord_client.hpp"

using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;
using namespace Aws::Http;

using namespace loabot::log;
using namespace loabot::handler;
using namespace  loabot::http;


using discord::client::AsyncDiscordClientBuilder;

int main(int, char**)
{
    Aws::InitAPI(Aws::SDKOptions());

    auto http_client = [] {
        Aws::Client::ClientConfiguration config;
        config.region = "ap-northeast-2";
        return CreateHttpClient(config);
    }();

    auto command_handler_builder = loabot::handler::LoabotHandlerBuilder();
    auto command_handler = std::shared_ptr<CommandHandler>(command_handler_builder.build(http_client));

    Aws::Client::ClientConfiguration aws_config;
    auto discord_builder = AsyncDiscordClientBuilder(aws_config);
    //

    run_handler([discord_builder, command_handler](const invocation_request& req) {
        LOG(req.payload);
        const auto payload = JsonValue(req.payload);

        try {
            auto discord = discord_builder.build(payload.View());

            const auto command = payload.View().GetObject("data").GetString("name");

            const auto result = command_handler->handle(command, payload.View());

            discord->reply(result);

            return invocation_response::success("Invoked!!!", "application/json");
        } catch (const std::exception& e) {
            LOG(e.what());
            return invocation_response::failure("", "");
        } catch (...) {
            LOG("unknown exception occurred");
            return invocation_response::failure("", "");
        }
    });

    return 0;
}