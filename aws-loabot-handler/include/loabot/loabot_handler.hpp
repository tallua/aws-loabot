#pragma once

#include <aws/core/client/ClientConfiguration.h>

#include "discord/discord_handler.hpp"

namespace loabot::handler {

struct LoabotBuilder {
    std::unique_ptr<discord::handler::DiscordHandler> build(
        Aws::Client::ClientConfiguration config);
};

}  // namespace loabot::handler