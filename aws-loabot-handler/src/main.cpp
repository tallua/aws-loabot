#include <aws/core/Aws.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/lambda-runtime/runtime.h>


#include <aws/core/http/HttpClientFactory.h>

#include "loabot/loabot_discord.hpp"
#include "loabot/loabot_handler.hpp"
#include "loabot/log.hpp"

using namespace loabot::log;

using aws::lambda_runtime::invocation_request;
using aws::lambda_runtime::invocation_response;
using aws::lambda_runtime::run_handler;
using Aws::Utils::Json::JsonValue;
using Aws::Utils::Json::JsonView;

using loabot::discord::AwsDiscordClient;
using loabot::discord::AwsDiscordContextData;

namespace {

struct Request {
    AwsDiscordContextData discord_context;
};

Request parse_request(JsonView view) {
    Request req;
    req.discord_context.url = view.GetObject("callback").GetString("url");
    req.discord_context.authorization = view.GetObject("callback").GetString("Authorization");
    req.discord_context.transaction_id = view.GetString("id");

    return req;
}

}  // namespace

using loabot::handler::LoabotBuilder;

int main(int, char**) {
    Aws::InitAPI(Aws::SDKOptions());

    auto aws_config = [] {
        Aws::Client::ClientConfiguration config;
        config.region = "ap-northeast-2";
        return config;
    }();

    auto http_client = Aws::Http::CreateHttpClient(aws_config);

    const auto discord_client = std::make_shared<AwsDiscordClient>(http_client);
    const auto loabot = std::shared_ptr(LoabotBuilder().build(http_client));

    run_handler([discord_client, loabot](const invocation_request& req) {
        LOG(req.payload);
        const auto payload = JsonValue(req.payload);

        try {
            auto request = parse_request(payload.View());

            auto discord_context = discord_client->create_context(
                request.discord_context
            );

            const auto command_name =
                payload.View().GetObject("data").GetString("name");

            const auto result = loabot->handle({command_name, payload.View().GetObject("data")});

            std::visit([&](auto&& req) { discord_context->reply(req); },
                       result);

            return invocation_response::success("Invoked!!!",
                                                "application/json");
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