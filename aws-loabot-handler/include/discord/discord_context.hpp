#pragma once

#include "discord_message.hpp"

namespace discord::context {

class DiscordContext {
public:
    DiscordContext() = default;
    virtual ~DiscordContext() = default;

public:
    void reply(const discord::message::Embed& embed) const;
    void reply(const discord::message::Content& content) const;

protected:
    virtual void send_data(const std::string& data) const = 0;
};

}