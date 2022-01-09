#include <aws/lambda-runtime/runtime.h>

#include <sstream>
#include <iostream>
#include <functional>
#include <optional>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/http/URI.h>

#include "discord/message.hpp"
#include "loabot/log.hpp"

using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;
using namespace Aws::Http;


using namespace loabot::log;

namespace loabot::http {

struct WebRequestParam {
    struct HeaderData {
        std::string key;
        std::string value;
    };
    URI uri;
    HttpMethod method;
    std::vector<HeaderData> headers;

    std::optional<std::string> authorization;
    std::optional<JsonValue> body;
};

std::shared_ptr<HttpRequest> create_request(const WebRequestParam& param)
{
    const auto request = CreateHttpRequest(param.uri, param.method, []() {
        return new std::stringstream();
    });

    std::for_each(param.headers.begin(), param.headers.end(), [&](auto&& data) {
        request->SetHeaderValue(data.key, data.value);
    });

    if (param.body.has_value()) {
        auto body_string = param.body.value().View().WriteCompact();
        request->SetContentLength(std::to_string(body_string.size()));
        request->AddContentBody(std::make_shared<std::stringstream>(std::move(body_string)));
        request->SetContentType("application/json");
    }

    if (param.authorization.has_value()) {
        request->SetAuthorization(param.authorization.value());
    }

    return request;
}

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

using namespace  loabot::http;
int main(int, char**)
{
    Aws::InitAPI(Aws::SDKOptions());

    auto http_client = [] {
        Aws::Client::ClientConfiguration config;
        config.region = "ap-northeast-2";
        return CreateHttpClient(config);
    }();

    run_handler([http_client](const invocation_request& req) {
        LOG(req.payload);
        const auto payload = JsonValue(req.payload);

        const auto discord_request = create_request({
            payload.View().GetObject("callback").GetString("url"),
            HttpMethod::HTTP_PATCH,
            {},
            payload.View().GetObject("callback").GetString("Authorization"),
            JsonValue(R"({ "content": "Yeah!!!" })")
        });

        const auto discord_response = http_client->MakeRequest(discord_request);
        const auto discord_response_body = [&]{
            std::stringstream ss;
            ss << discord_response->GetResponseBody().rdbuf();
            return ss.str();
        }();
        LOG("Response Code: (", int(discord_response->GetResponseCode()), ") : ", discord_response_body);

        return invocation_response::success("Invoked!!!", "application/json");
    });

    return 0;
}