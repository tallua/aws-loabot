#include "discord/discord_client.hpp"

#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpRequest.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <loabot/log.hpp>
#include <sstream>

namespace {
std::shared_ptr<Aws::Http::HttpRequest> create_request(
    const std::string& url, const std::string& auth,
    Aws::Utils::Json::JsonView body) {
    // TODO
    return nullptr;
}

Aws::Utils::Json::JsonValue format(const discord::message::Embed&) {
    // TODO
    return {};
}
Aws::Utils::Json::JsonValue format(const discord::message::Content&) {
    // TODO
    return {};
}

}  // namespace

using namespace loabot::log;

namespace discord::client {

struct AsyncDiscordClient::Context {
    using HttpClient = Aws::Http::HttpClient;

    std::string url;
    std::string authorization;
    std::string transaction_id;
    std::shared_ptr<HttpClient> http_client;
};

AsyncDiscordClient::AsyncDiscordClient()
    // TODO
    : context(std::make_unique<Context>(Context{"", "", "", nullptr})) {}

AsyncDiscordClient::~AsyncDiscordClient() = default;

void AsyncDiscordClient::reply(const discord::message::Embed& embed) {
    const auto discord_request = create_request(
        context->url, context->authorization, ::format(embed).View());

    const auto discord_response =
        context->http_client->MakeRequest(discord_request);
    const auto discord_response_body = [&] {
        std::stringstream ss;
        ss << discord_response->GetResponseBody().rdbuf();
        return ss.str();
    }();

    LOG("Discord Response: transaction[", context->transaction_id, "] (",
        int(discord_response->GetResponseCode()),
        ") : ", discord_response_body);
}

void AsyncDiscordClient::reply(const discord::message::Content& content) {
    const auto discord_request = create_request(
        context->url, context->authorization, ::format(content).View());

    const auto discord_response =
        context->http_client->MakeRequest(discord_request);
    const auto discord_response_body = [&] {
        std::stringstream ss;
        ss << discord_response->GetResponseBody().rdbuf();
        return ss.str();
    }();

    LOG("Discord Response: transaction[", context->transaction_id, "] (",
        int(discord_response->GetResponseCode()),
        ") : ", discord_response_body);
}

}  // namespace discord::client