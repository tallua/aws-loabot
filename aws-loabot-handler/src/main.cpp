#include <aws/core/Aws.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/lambda-runtime/runtime.h>

#include "discord/discord_client.hpp"
#include "loabot/loabot_handler.hpp"
#include "loabot/log.hpp"

using namespace loabot::log;

using aws::lambda_runtime::invocation_request;
using aws::lambda_runtime::invocation_response;
using aws::lambda_runtime::run_handler;
using Aws::Utils::Json::JsonValue;

using discord::client::AsyncDiscordClientBuilder;

using loabot::handler::LoabotBuilder;

int main(int, char**) {
    Aws::InitAPI(Aws::SDKOptions());

    auto aws_config = [] {
        Aws::Client::ClientConfiguration config;
        config.region = "ap-northeast-2";
        return config;
    }();

    const auto discord_builder = AsyncDiscordClientBuilder(aws_config);
    const auto loabot = std::shared_ptr(LoabotBuilder().build(aws_config));

    run_handler([&discord_builder, loabot](const invocation_request& req) {
        LOG(req.payload);
        const auto payload = JsonValue(req.payload);

        try {
            auto discord = discord_builder.build(payload.View());

            const auto command =
                payload.View().GetObject("data").GetString("name");

            using discord::handler::Command;
            const auto result =
                loabot->handle(Command{command, payload.View()});

            std::visit([&](auto&& req) { discord->reply(req); }, result);

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