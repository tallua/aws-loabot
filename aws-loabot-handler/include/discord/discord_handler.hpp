#pragma once

#include <aws/core/utils/json/JsonSerializer.h>

#include <variant>
#include <functional>

#include "discord_message.hpp"

namespace discord::handler {

struct Request {
    Aws::Utils::Json::JsonView view;
};

struct Command {
    const std::string& command;
    Aws::Utils::Json::JsonView payload;
};

using Response =
    std::variant<discord::message::Content, discord::message::Embed>;

class DiscordHandler {
public:
    using CommandHandler = std::function<Response(const Command&)>;

public:
    Response handle(Command command) const {
        auto h_it = handlers.find(command.command);
        if (h_it == handlers.end()) {
            throw std::runtime_error("Unknown Command: " + command.command);
        }

        if (!h_it->second) {
            throw std::runtime_error("Null Handler " + command.command);
        }

        return h_it->second(command);
    }

    void add_command(const std::string& command, CommandHandler handler) {
        handlers[command] = handler;
    }

private:
    std::unordered_map<std::string, CommandHandler> handlers;
};

}  // namespace discord::handler