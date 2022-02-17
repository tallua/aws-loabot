#include "discord/discord_context.hpp"

#include "aws/core/utils/json/JsonSerializer.h"

namespace {

Aws::Utils::Json::JsonValue format_embed(const discord::message::Embed&) {
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

}

namespace discord::context {

void DiscordContext::reply(const discord::message::Embed& embed) const
{
    send_data(::format(embed).View().WriteCompact());
}

void DiscordContext::reply(const discord::message::Content& content) const
{
    send_data(::format(content).View().WriteCompact());
}


}