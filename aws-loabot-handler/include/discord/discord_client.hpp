#pragma once

#include <memory>

#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include "discord_message.hpp"

namespace discord::client {

class AsyncDiscordClient {

public:
    AsyncDiscordClient();
    ~AsyncDiscordClient();

public:
    void reply(const discord::message::Embed& embed);
    void reply(const discord::message::Content& content);

private:
    struct Context;
    std::unique_ptr<Context> context;
};

class AsyncDiscordClientBuilder {
public:
    using DiscordClient = std::unique_ptr<AsyncDiscordClient>;
    using Json = Aws::Utils::Json::JsonView;

public:
    AsyncDiscordClientBuilder(Aws::Client::ClientConfiguration);

    DiscordClient build(Json request) const;

    Aws::Client::ClientConfiguration config;
};

}