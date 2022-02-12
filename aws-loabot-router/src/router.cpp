#include "loabot/router.hpp"

#include "loabot/log.hpp"

using namespace loabot::log;
using namespace loabot::route;
using namespace Aws::Utils::Json;

void Router::add_handler(std::string command, RouteHandler handler) {
    route_table[std::move(command)] = handler;
}

void Router::set_fail_response(discord::Response response) {
    fail_response = response;
}

discord::Response Router::route(const JsonView body,
                                const RouteContext& context) const {
    const auto command = body.GetObject("data").GetString("name");
    auto handler_it = route_table.find(command);
    if (handler_it == route_table.end()) {
        LOG("Failed to find command: ", command);
        return fail_response;
    }

    if (!handler_it->second) {
        LOG("Invalid Handler: ", command);
        return fail_response;
    }

    return handler_it->second(body, context);
}
