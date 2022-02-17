#include "loabot/loabot_discord.hpp"

#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/http/HttpRequest.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <loabot/log.hpp>
#include <sstream>

namespace {

std::shared_ptr<Aws::Http::HttpRequest> create_request(
    const std::string& url, const std::string& auth, const std::string& body) {
    auto request =
        Aws::Http::CreateHttpRequest(url, Aws::Http::HttpMethod::HTTP_PATCH,
                                     []() { return new std::stringstream(); });

    request->SetContentLength(std::to_string(body.size()));
    request->AddContentBody(
        std::make_shared<std::stringstream>(std::move(body)));
    request->SetContentType("application/json");
    request->SetAuthorization(auth);

    return request;
}

}  // namespace

using namespace loabot::log;

namespace loabot::discord {

void AwsDiscordContext::send_data(const std::string& body) const {
    const auto discord_request =
        create_request(context.url, context.authorization, body);

    LOG("Responding: ", body);
    const auto discord_response = http_client->MakeRequest(discord_request);
    const auto discord_response_body = [&] {
        std::stringstream ss;
        ss << discord_response->GetResponseBody().rdbuf();
        return ss.str();
    }();

    LOG("Discord Response: transaction[", context.transaction_id, "] (",
        int(discord_response->GetResponseCode()),
        ") : ", discord_response_body);
}

std::unique_ptr<AwsDiscordContext> AwsDiscordClient::create_context(
    AwsDiscordContextData data) {
    return std::make_unique<AwsDiscordContext>(
        AwsDiscordContext(data, http_client));
}

}  // namespace loabot::discord