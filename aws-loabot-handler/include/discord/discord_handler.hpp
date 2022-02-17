#pragma once

#include <functional>
#include <variant>

#include "discord_message.hpp"

namespace discord::handler {

template <typename _Tp>
struct DiscordCommand {
    std::string command;
    _Tp payload;
};

using DiscordResponse =
    std::variant<discord::message::Content, discord::message::Embed>;

template <typename _Tp>
class DiscordHandler {
public:
    using Command = DiscordCommand<_Tp>;
    using CommandHandler = std::function<DiscordResponse(const Command&)>;

public:
    DiscordResponse handle(const Command& command) const {
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