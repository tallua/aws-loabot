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
#include "loabot/handler.hpp"
#include "loabot/http.hpp"

using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;
using namespace Aws::Http;


using namespace loabot::log;
using namespace loabot::handler;
using namespace  loabot::http;

namespace {

void test_lostark_webpage_get(std::shared_ptr<HttpClient> http_client)
{
    const auto lostark_request = create_request({
        "",
        HttpMethod::HTTP_GET,
        {},
        {},
        {}
    });

    const auto http_response = http_client->MakeRequest(lostark_request);
    const auto http_response_code = http_response->GetResponseCode();
    if (http_response_code == HttpResponseCode::OK) {
        const auto http_response_body = [&]{
            std::stringstream ss;
            ss << http_response->GetResponseBody().rdbuf();
            return ss.str();
        }();
        LOG("Response Code: (", int(http_response->GetResponseCode()), ") : Length: ", http_response_body.size());
    } else {
        LOG("Error: http status code(", int(http_response->GetResponseCode()), ")");
    }
}

}

int main(int, char**)
{
    Aws::InitAPI(Aws::SDKOptions());

    auto http_client = [] {
        Aws::Client::ClientConfiguration config;
        config.region = "ap-northeast-2";
        return CreateHttpClient(config);
    }();

    auto command_handler_builder = loabot::handler::LoabotHandlerBuilder();
    auto command_handler = std::shared_ptr<CommandHandler>(command_handler_builder.build());

    run_handler([http_client, command_handler](const invocation_request& req) {
        LOG(req.payload);
        const auto payload = JsonValue(req.payload);

        const auto transaction_id = payload.View().GetString("id");
        const auto command = payload.View().GetObject("data").GetString("name");

        auto discord_result = JsonValue(R"({ "content": "오류가 생겼어요. 나중에 다시 시도해주세요" })");
        try {
            discord_result = command_handler->handle(command, payload.View().GetObject("data"));
        }
        catch (const std::exception& e){
            LOG("Error: failed to handle: transaction[", transaction_id, "] ", e.what());
        }

        const auto discord_request = create_request({
            payload.View().GetObject("callback").GetString("url"),
            HttpMethod::HTTP_PATCH,
            {},
            payload.View().GetObject("callback").GetString("Authorization"),
            discord_result
        });

        const auto discord_response = http_client->MakeRequest(discord_request);
        const auto discord_response_body = [&]{
            std::stringstream ss;
            ss << discord_response->GetResponseBody().rdbuf();
            return ss.str();
        }();
        LOG("Discord Response: transaction[", transaction_id, "] (", int(discord_response->GetResponseCode()), ") : ", discord_response_body);

        return invocation_response::success("Invoked!!!", "application/json");
    });

    return 0;
}