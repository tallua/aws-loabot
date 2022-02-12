#include <gtest/gtest.h>

#include "discord/validate.hpp"

namespace {

// ed25519 for testing:
// private key as base64:
// mKreBKSt+ewRcgQgVJB+nRZE8Kj/aumhzzJpELjFLMN83kVEaUJR1pSsV68mLyHS0jZASL28lvBx8sRirSbfrg==
// public key as base64:
// fN5FRGlCUdaUrFevJi8h0tI2QEi9vJbwcfLEYq0m364=

TEST(Validator, validate_pass) {
    using namespace Aws::Utils::Json;

    // clang-format off
    auto message = JsonValue()
        .WithObject("headers", JsonValue()
            .WithString("x-signature-timestamp", "1234")
            .WithString("x-signature-ed25519", "9588c19a7a02a4aaa140fc4a53f1cf1652819eeed16501254ca8eac6aa9489baff4f8f3e38c9d17eb0eb91f60a49159cd92210b3270611eae25f64233056f508"))
        .WithObject("stageVariables", JsonValue()
            .WithString("DISCORD_APP_KEY", "7cde4544694251d694ac57af262f21d2d2364048bdbc96f071f2c462ad26dfae"))
        .WithString("body", "helloworld");
    // clang-format on

    EXPECT_TRUE(discord::validate_message(message));
}

TEST(Validator, validate_fail) {
    using namespace Aws::Utils::Json;

    // clang-format off
    auto message = JsonValue()
        .WithObject("headers", JsonValue()
            .WithString("x-signature-timestamp", "1234")
            .WithString("x-signature-ed25519", "b80745ec470c52fbd08bb1e6c3b28dfaae3f6ad54bea885bff51af35dcff9df5642921d0be1642eb421afdb825b812b72d932578022dda2c11a196ca4ae9960d"))
        .WithObject("stageVariables", JsonValue()
            .WithString("DISCORD_APP_KEY", "7cde4544694251d694ac57af262f21d2d2364048bdbc96f071f2c462ad26dfae"))
        .WithString("body", "helloworld");
    // clang-format on

    EXPECT_FALSE(discord::validate_message(message));
}

}  // namespace