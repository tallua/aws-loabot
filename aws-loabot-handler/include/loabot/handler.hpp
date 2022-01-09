#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

#include <aws/core/utils/json/JsonSerializer.h>

namespace loabot::handler {

struct CommandHandler
{
    using JsonValue = Aws::Utils::Json::JsonValue;
    using JsonView = Aws::Utils::Json::JsonView;
    using Handler = std::function<JsonValue(const JsonView)>;

    JsonValue handle(const std::string& command, const JsonView data) {
        auto h_it = handlers.find(command);
        if (h_it == handlers.end()) {
            throw std::runtime_error("Unknown Command: " + command);
        }

        if (!h_it->second) {
            throw std::runtime_error("Null Handler " + command);
        }

        return h_it->second(data);
    }

    void add_handler(const std::string& command, Handler handler) {
        handlers[command] = handler;
    }

private:
    std::unordered_map<std::string, Handler> handlers;
};

struct LoabotHandlerBuilder {
    std::unique_ptr<CommandHandler> build();
};

}