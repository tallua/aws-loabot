#include "aws/core/utils/json/JsonSerializer.h"
#include "discord/discord_context.hpp"

namespace {

Aws::Utils::Json::JsonValue format_embed(const discord::message::Embed& embed) {
    using Aws::Utils::Json::JsonValue;

    JsonValue result;

    if (embed.title.has_value()) {
        result.WithString("title", embed.title.value());
    }
    if (embed.description.has_value()) {
        result.WithString("description", embed.description.value());
    }
    if (embed.url.has_value()) {
        result.WithString("url", embed.url.value());
    }
    if (embed.thumbnail.has_value()) {
        result.WithObject("thumbnail", JsonValue().WithString(
                                           "url", embed.thumbnail.value()));
    }

    if (!embed.fields.empty()) {
        Aws::Utils::Array<JsonValue> fields(embed.fields.size());

        for(std::size_t i = 0; i < embed.fields.size(); ++i) {
            fields[i].WithString("name", embed.fields[i].name);
            fields[i].WithString("value", embed.fields[i].value);
            fields[i].WithBool("inline", embed.fields[i].inlined);
        }

        result.WithArray("fields", fields);
    }

    return result;
}

Aws::Utils::Json::JsonValue format(const discord::message::Embed& embed) {
    using Aws::Utils::Json::JsonValue;

    Aws::Utils::Array<JsonValue> embeds(1);
    embeds[0] = format_embed(embed);

    // clang-format off
    return JsonValue()
        .WithBool("tts", false)
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
        .WithObject("allowed_mentions", JsonValue()
            .WithArray("parse", Aws::Utils::Array<JsonValue>())
            .WithArray("roles", Aws::Utils::Array<JsonValue>())
            .WithArray("users", Aws::Utils::Array<JsonValue>())
            .WithBool("replied_user", false))
        .WithInteger("flags", 0);
    // clang-format on
}

}  // namespace

namespace discord::context {

void DiscordContext::reply(const discord::message::Embed& embed) const {
    send_data(::format(embed).View().WriteCompact());
}

void DiscordContext::reply(const discord::message::Content& content) const {
    send_data(::format(content).View().WriteCompact());
}

}  // namespace discord::context