#include <iostream>
#include <string>

#include <aws/lambda-runtime/runtime.h>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include "discord/validate.hpp"

using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;

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

}

int main(int, char**)
{
    Aws::InitAPI(Aws::SDKOptions());

    aws::lambda_runtime::run_handler([](const invocation_request& req) {
        LOG(req.payload);
        const JsonValue payload(req.payload);

        if (!discord::validate_message(payload.View())) {
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