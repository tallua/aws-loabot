#pragma once

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include "discord_message.hpp"

namespace discord::client {

struct AsyncDiscordClient {
    void reply(const discord::message::Embed& embed);
    void reply(const discord::message::Content& content);
};

class AsyncDiscordClientBuilder {
public:
    using ClientPtr = std::unique_ptr<AsyncDiscordClient>;
    using Json = Aws::Utils::Json::JsonView;
public:
    AsyncDiscordClientBuilder(Aws::Client::ClientConfiguration);

    ClientPtr build(Json request) const;
};

}