#pragma once

#include <aws/core/http/HttpClient.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include "discord/discord_handler.hpp"

namespace loabot::handler {

struct LoabotBuilder {
    using JsonView = Aws::Utils::Json::JsonView;
    using Handler = ::discord::handler::DiscordHandler<JsonView>;
    using HandlerPtr = std::unique_ptr<Handler>;

    HandlerPtr build(std::shared_ptr<Aws::Http::HttpClient> http);
};

}  // namespace loabot::handler