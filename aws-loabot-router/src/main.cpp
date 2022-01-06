#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include <aws/lambda-runtime/runtime.h>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>

#include <aws/secretsmanager/SecretsManagerClient.h>
#include <aws/secretsmanager/SecretsManagerRequest.h>
#include <aws/secretsmanager/model/GetSecretValueRequest.h>

using namespace std;
using namespace aws::lambda_runtime;
using namespace Aws::Utils::Json;
using namespace Aws::SecretsManager;
using namespace Aws::SecretsManager::Model;

struct LambdaConfig
{
    std::string discord_secretsmanager;
};

LambdaConfig load_config(const std::string& path)
{
    std::ifstream config_file(path);
    const JsonValue config(config_file);

    return LambdaConfig {
        config.View().GetObject("discord").GetString("scretsmanager")
    };
}

void init()
{
    Aws::InitAPI(Aws::SDKOptions());

    const auto config = load_config("./config.json");

    auto client = Aws::MakeShared<SecretsManagerClient>("loabot-router-secret",
        Aws::Client::ClientConfiguration());
    auto value = client->GetSecretValue(
        GetSecretValueRequest().WithSecretId(config.discord_secretsmanager));
    cout << "Error: " << value.GetError() << endl;
    cout << "Secrets: " << config.discord_secretsmanager << endl;
}

int main(int, char**)
{
    init();

    aws::lambda_runtime::run_handler([](auto&&) {
        return invocation_response::failure("Not Implemented", "500");
    });

    return 0;
}