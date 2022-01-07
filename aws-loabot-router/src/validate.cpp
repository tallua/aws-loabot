#include "discord/validate.hpp"

#include <vector>
#include <string>
#include <algorithm>

#include <sodium.h>

using namespace Aws::Utils::Json;

namespace {

unsigned char hex_to_number(char ch)
{
    if ('0' <= ch && ch <= '9')
    {
        return ch - '0';
    }
    if ('a' <= ch && ch <= 'f')
    {
        return ch - 'a' + 10;
    }
    if ('A' <= ch && ch <= 'F')
    {
        return ch - 'A' + 10;
    }
    throw std::runtime_error("invalid hex character");
}

bool validate(const std::string& sign, const std::string& message, const std::string& key)
{
    auto from_str = [](const std::string& str) {
        std::vector<unsigned char> hex(str.size());
        std::transform(str.begin(), str.end(), hex.begin(), [](auto ch) {
            return static_cast<unsigned char>(ch);
        });
        return hex;
    };
    auto from_hex = [](const std::string& str) {
        std::vector<unsigned char> hex(str.size() / 2);
        for (size_t i = 0; i < str.size(); i += 2)
        {
            hex[i / 2] = (hex_to_number(str[i]) * 0x10 + hex_to_number(str[i + 1]));
        }
        return hex;
    };

    const auto sign_hex = from_hex(sign);
    const auto message_hex = from_str(message);
    const auto key_hex = from_hex(key);

    return crypto_sign_verify_detached(sign_hex.data(),
        message_hex.data(), message_hex.size(), key_hex.data()) == 0;
}

}

bool discord::validate_message(const JsonView& req)
{
    const auto body = req.GetString("body");
    const auto timestamp = req.GetObject("headers").GetString("x-signature-timestamp");
    const auto ed25519 = req.GetObject("headers").GetString("x-signature-ed25519");
    const auto key = req.GetObject("stageVariables").GetString("DISCORD_APP_KEY");

    return validate(ed25519, timestamp + body, key);
}