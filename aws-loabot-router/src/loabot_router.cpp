#include "loabot/router.hpp"

#include <random>

#include "loabot/log.hpp"

using namespace loabot::log;
using namespace loabot::route;
using namespace Aws::Utils::Json;

struct LoabotContext
{
    std::random_device random;
};

std::unique_ptr<Router> LoabotRouterBuilder::build()
{
    auto router = std::make_unique<Router>();
    auto context = std::make_shared<LoabotContext>();

    router->add_handler("주사위", [context](auto&&, auto&&) {
        auto dist = std::uniform_int_distribution<int>(0, 100);
        const auto random_value = dist(context->random);
        return discord::ResponseBuilder::Message("주사위를 굴려서 " + std::to_string(random_value) + "이 나왔어요.");
    });

    return router;
}

