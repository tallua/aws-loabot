#include <aws/lambda-runtime/runtime.h>

#include <sstream>
#include <functional>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpClientFactory.h>

#include "discord/message.hpp"
#include "loabot/log.hpp"

using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;
using namespace loabot::log;

int main(int, char**)
{
    Aws::InitAPI(Aws::SDKOptions());

    run_handler([](const invocation_request& req) {
        LOG(req.payload);
        const auto payload = JsonValue(req.payload);

        return invocation_response::success("Invoked!!!", "application/json");
    });

    return 0;
}