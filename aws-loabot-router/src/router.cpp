#include "loabot/router.hpp"

#include "loabot/log.hpp"


using namespace loabot::log;
using namespace loabot::route;

void Router::add_handler(std::string command, RouteHandler handler)
{
    route_table[std::move(command)] = handler;
}

discord::Response Router::route(const JsonView body, const RouteContext& context) const
{
    const auto command = body.GetObject("data").GetString("name");
    auto handler_it = route_table.find(command);
    if (handler_it == route_table.end()) {
        LOG("Failed to find command: ", command);
        return discord::ResponseBuilder::Failed("Unknown Command");
    }

    if (!handler_it->second) {
        LOG("Invalid Handler: ", command);
        return discord::ResponseBuilder::Failed("Invalid Handler");
    }

    return handler_it->second(body, context);
}

std::unique_ptr<Router> LoabotRouterBuilder::build()
{
    auto router = std::make_unique<Router>();

    return router;
}

