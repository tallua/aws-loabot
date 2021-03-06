#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

#include "discord/message.hpp"

#include <aws/core/utils/json/JsonSerializer.h>

namespace loabot::route {

struct RouteContext
{
    std::string api_id;
    std::string bot_token;
    std::string handler_prefix;
    std::string stage_name;
};

class Router {
public:
    using JsonView = Aws::Utils::Json::JsonView;
    using RouteHandler = std::function<discord::Response(const JsonView, const RouteContext&)>;

public:
    void add_handler(std::string command, RouteHandler handler);
    void set_fail_response(discord::Response response);

    discord::Response route(const JsonView body, const RouteContext& context) const;

private:
    using RoutingTable = std::unordered_map<std::string, RouteHandler>;
    discord::Response fail_response;
    RoutingTable route_table;
};

struct LoabotRouterBuilder {
    std::unique_ptr<Router> build();
};

}