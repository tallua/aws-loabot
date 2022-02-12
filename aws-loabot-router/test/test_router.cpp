#include <gtest/gtest.h>

#include "loabot/router.hpp"

namespace {

TEST(LoabotRouter, route_success) {
    using namespace loabot::route;
    using namespace Aws::Utils::Json;

    const auto expected =
        discord::ResponseBuilder::Message("this is test message");

    // clang-format off
    const auto request = JsonValue()
        .WithObject("data", JsonValue()
            .WithString("name", "ping")
        );
    // clang-format on

    auto router = [&] {
        Router router;
        router.add_handler("ping", [&](auto&&, auto&&) { return expected; });

        return router;
    }();
    const auto response = router.route(request, RouteContext());

    EXPECT_EQ(expected.to_string(), response.to_string());
}

TEST(LoabotRouter, route_null) {
    using namespace loabot::route;
    using namespace Aws::Utils::Json;

    const auto expected = discord::Response{"this is test message"};

    // clang-format off
    const auto request = JsonValue()
        .WithObject("data", JsonValue()
            .WithString("name", "ping")
        );
    // clang-format on

    auto router = [] {
        Router router;
        router.add_handler("ping", nullptr);

        return router;
    }();
    const auto response = router.route(request, RouteContext());

    EXPECT_EQ(discord::ResponseBuilder::Failed("Invalid Handler").to_string(),
              response.to_string());
}

TEST(LoabotRouter, route_fail) {
    using namespace loabot::route;
    using namespace Aws::Utils::Json;

    // clang-format off
    const auto request = JsonValue()
        .WithObject("data", JsonValue()
            .WithString("name", "ping")
        );
    // clang-format on
    auto router = Router();
    const auto response = router.route(request, RouteContext());

    EXPECT_EQ(discord::ResponseBuilder::Failed("Unknown Command").to_string(),
              response.to_string());
}

}  // namespace
