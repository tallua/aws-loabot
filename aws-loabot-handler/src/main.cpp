#include <aws/lambda-runtime/runtime.h>

#include <sstream>
#include <iostream>
#include <functional>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpClientFactory.h>

#include "discord/message.hpp"
#include "loabot/log.hpp"

using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;
using namespace Aws::Http;

using namespace loabot::log;

int main(int, char**)
{
    Aws::InitAPI(Aws::SDKOptions());

    auto client = [] {
        Aws::Client::ClientConfiguration config;
        config.region = "ap-northeast-2";
        return CreateHttpClient(config);
    }();

    run_handler([client](const invocation_request& req) {
        LOG(req.payload);

        const auto payload = JsonValue(req.payload);

        const auto response_url = payload.View().GetObject("callback").GetString("url");
        const auto response_auth = payload.View().GetObject("callback").GetString("Authorization");
        const auto discord_request = CreateHttpRequest(response_url, HttpMethod::HTTP_PATCH, []() {
            return new std::stringstream();
        });
        discord_request->SetHeaderValue("Authorization", response_auth);
        discord_request->SetAuthorization(response_auth);
        discord_request->SetContentType("application/json");
        const std::string test_data = R"({ "content": "Yeah!!!" })";
        auto request_body = [&] {
            LOG(test_data);
            auto stream = std::make_shared<std::stringstream>(test_data);
            return stream;
        }();
        discord_request->AddContentBody(request_body);
        discord_request->SetContentLength(std::to_string(test_data.size()));

        const auto discord_response = client->MakeRequest(discord_request);
        LOG("Response Code: ", int(discord_response->GetResponseCode()));
        std::stringstream ss;
        ss << discord_response->GetResponseBody().rdbuf();
        const auto response_body = ss.str();
        LOG("Response Body: ", response_body);

        return invocation_response::success("Invoked!!!", "application/json");
    });

    return 0;
}