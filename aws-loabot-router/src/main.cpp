#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <vector>

#include <aws/lambda-runtime/runtime.h>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <aws/secretsmanager/SecretsManagerClient.h>
#include <aws/secretsmanager/SecretsManagerRequest.h>
#include <aws/secretsmanager/model/GetSecretValueRequest.h>

#include <sodium.h>

using namespace std;
using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;
using namespace Aws::SecretsManager;
using namespace Aws::SecretsManager::Model;


namespace {

template <typename _Tp>
void tlog(_Tp&& arg) { std::cout << arg; }

template <typename _First, typename... _Rest>
void tlog(_First&& first, _Rest&&... rest)
{
    std::cout << first;
    tlog(std::forward<_Rest>(rest)...);
}

template <typename..._Args>
void LOG(_Args&&... args)
{
    tlog(std::forward<_Args>(args)...);
    std::cout << std::endl;
}

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


bool validate(const JsonView& req)
{
    const auto body = req.GetString("body");
    const auto timestamp = req.GetObject("headers").GetString("x-signature-timestamp");
    const auto ed25519 = req.GetObject("headers").GetString("x-signature-ed25519");
    const auto key = req.GetObject("stageVariables").GetString("DISCORD_APP_KEY");

    if (!validate(ed25519, timestamp + body, key)) {
        LOG("Error: verification failed");
        return false;
    }

    return true;
}

void init()
{
    Aws::InitAPI(Aws::SDKOptions());
}

int main(int, char**)
{
    init();

    aws::lambda_runtime::run_handler([](const invocation_request& req) {
        LOG(req.payload);
        const JsonValue payload(req.payload);

        if (!validate(payload.View())) {
            LOG("Error: validation failed");
            return invocation_response::failure("Invalid Request", "400");
        }

        LOG("Success: validation success");
        return invocation_response::success(R"({
            "statusCode": 200,
            "isBase64Encoded": false,
            "headers": {
                "content-type": "application/json"
            },
            "body": "{\"type\": 1 }"
        })", "application/json");
    });

    return 0;
}