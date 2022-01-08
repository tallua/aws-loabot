#include "loabot/router.hpp"

#include <random>
#include <exception>

#include "loabot/log.hpp"

#include <aws/lambda/LambdaClient.h>
#include <aws/lambda/model/InvokeRequest.h>

using namespace loabot::log;
using namespace loabot::route;
using namespace Aws::Utils::Json;
using namespace Aws::Lambda;
using namespace Aws::Lambda::Model;

#ifndef LOABOT_LAMBDA_CLIENT 
#define LOABOT_LAMBDA_CLIENT "LoabotRouterLambda"
#endif

namespace {

struct LoabotHandlers
{


    std::string find_handler(const std::string& command, const RouteContext& context) const
    {
        auto h_it = handler_postfixes.find(command);
        if (h_it == handler_postfixes.end()) {
            throw std::runtime_error("Unknown Command Mapping: " + command);
        }

        return context.handler_prefix + h_it->second;
    }

private:
    const std::unordered_map<std::string, std::string> handler_postfixes = {
        { "거래소", "-Handler1" },
        { "수집품", "-Handler1" },
        { "캐릭터", "-Handler1" }
    };

};

void invoke_lambda(std::shared_ptr<LambdaClient> client, const std::string& name, const JsonView body, const RouteContext& context)
{
    InvokeRequest request;
    request.SetFunctionName(name);
    request.SetInvocationType(InvocationType::Event);
    request.SetLogType(LogType::Tail);
    auto request_content = [&] {
        const auto app_id = body.GetString("application_id");
        const auto interaction_token = body.GetString("token");
        auto stream = std::make_shared<Aws::StringStream>();
        auto new_payload = body.Materialize();
        new_payload.WithObject("callback", JsonValue()
            .WithString("url", "https://discord.com/api/webhooks/" + app_id + "/" + interaction_token + "/messages/@original")
            .WithString("Authorization", "Bot " + context.bot_token));
        *stream << new_payload.View().WriteCompact();

        return stream;
    }();
    request.SetBody(request_content);
    request.SetContentType("application/json");

    auto response = client->Invoke(request);
    if (!response.IsSuccess()) {
        LOG("Error: ", int(response.GetError().GetErrorType()), ": ", response.GetError().GetMessage());
        throw std::runtime_error(response.GetError().GetMessage());
    }

    LOG("Success calling: ", name);
}

}

std::unique_ptr<Router> LoabotRouterBuilder::build()
{
    auto router = std::make_unique<Router>();

    auto handlers = std::make_shared<LoabotHandlers>();
    auto random = std::make_shared<std::random_device>();

    auto lambda_client_config = Aws::Client::ClientConfiguration();
    lambda_client_config.region = "ap-northeast-2";
    auto lambda_client = Aws::MakeShared<LambdaClient>(LOABOT_LAMBDA_CLIENT, lambda_client_config);

    router->add_handler("주사위", [random](auto&&, auto&&) {
        auto dist = std::uniform_int_distribution<int>(0, 100);
        const auto random_value = dist(*random);
        LOG("Generated value: ", random_value);
        return discord::ResponseBuilder::Message("주사위를 굴려서 " + std::to_string(random_value) + "이 나왔어요.");
    });
    router->add_handler("거래소", [handlers, lambda_client](const JsonView body, const RouteContext& context) {
        const auto lambda = handlers->find_handler("거래소", context);
        LOG("Invoking: ", lambda, " for 거래소");
        invoke_lambda(lambda_client, lambda, body, context);
        return discord::ResponseBuilder::Failed("준비중이에요.");
    });
    router->add_handler("수집품", [handlers, lambda_client](const JsonView body, const RouteContext& context) {
        const auto lambda = handlers->find_handler("수집품", context);
        LOG("Invoking: ", lambda, " for 수집품");
        invoke_lambda(lambda_client, lambda, body, context);
        return discord::ResponseBuilder::Failed("준비중이에요.");
    });
    router->add_handler("캐릭터", [handlers, lambda_client](const JsonView body, const RouteContext& context) {
        const auto& lambda = handlers->find_handler("캐릭터", context);
        LOG("Invoking: ", lambda, " for 캐릭터");
        invoke_lambda(lambda_client, lambda, body, context);
        return discord::ResponseBuilder::Pending();
    });
    router->add_handler("핑", [](auto&&, auto&&) {
        return discord::ResponseBuilder::Message("퐁!");
    });

    return router;
}

