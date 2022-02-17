#include "discord/discord_client.hpp"

#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/http/HttpRequest.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <loabot/log.hpp>
#include <sstream>

namespace {

std::shared_ptr<Aws::Http::HttpRequest> create_request(
    const std::string& url, const std::string& auth,
    Aws::Utils::Json::JsonView body) {

    auto request =
        Aws::Http::CreateHttpRequest(url, Aws::Http::HttpMethod::HTTP_PATCH,
                                     []() { return new std::stringstream(); });

    auto body_string = body.WriteCompact();
    request->SetContentLength(std::to_string(body_string.size()));
    request->AddContentBody(
        std::make_shared<std::stringstream>(std::move(body_string)));
    request->SetContentType("application/json");
    request->SetAuthorization(auth);

    return request;
}

Aws::Utils::Json::JsonValue format_embed(const discord::message::Embed& embed) {
    using Aws::Utils::Json::JsonValue;

    // TODO
    // clang-format off
    return JsonValue();
    // clang-format on
}

Aws::Utils::Json::JsonValue format(const discord::message::Embed& embed) {
    using Aws::Utils::Json::JsonValue;

    Aws::Utils::Array<JsonValue> embeds(1);
    embeds[0] = format_embed(embed);

    // clang-format off
    return JsonValue()
        .WithBool("tts", false)
        .WithString("content", "")
        .WithArray("embeds", std::move(embeds))
        .WithObject("allowed_mentions", JsonValue()
            .WithArray("parse", Aws::Utils::Array<JsonValue>())
            .WithArray("roles", Aws::Utils::Array<JsonValue>())
            .WithArray("users", Aws::Utils::Array<JsonValue>())
            .WithBool("replied_user", false))
        .WithInteger("flags", 0);
    // clang-format on
}

Aws::Utils::Json::JsonValue format(const discord::message::Content& content) {
    using Aws::Utils::Json::JsonValue;

    // clang-format off
    return JsonValue()
        .WithBool("tts", false)
        .WithString("content", content.data)
        .WithArray("embeds", Aws::Utils::Array<JsonValue>())
        .WithObject("allowed_mentions", JsonValue()
            .WithArray("parse", Aws::Utils::Array<JsonValue>())
            .WithArray("roles", Aws::Utils::Array<JsonValue>())
            .WithArray("users", Aws::Utils::Array<JsonValue>())
            .WithBool("replied_user", false))
        .WithInteger("flags", 0);
    // clang-format on
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