#pragma once

#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include "discord/discord_handler.hpp"

namespace loabot::handler {

struct LoabotBuilder {
    using JsonView = Aws::Utils::Json::JsonView;
    using Handler = ::discord::handler::DiscordHandler<JsonView>;
    using HandlerPtr = std::unique_ptr<Handler>;

    HandlerPtr build(Aws::Client::ClientConfiguration config);
};

}  // namespace loabot::handler