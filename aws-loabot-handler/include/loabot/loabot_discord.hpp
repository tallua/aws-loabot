#pragma once

#include <aws/core/http/HttpClient.h>

#include <memory>
#include <string>

#include "discord/discord_context.hpp"

namespace loabot::discord {

struct AwsDiscordContextData {
    std::string url;
    std::string authorization;
    std::string transaction_id;
};

class AwsDiscordContext : public ::discord::context::DiscordContext {
protected:
    void send_data(const std::string& body) const override;

private:
    friend class AwsDiscordClient;
    AwsDiscordContext(AwsDiscordContextData context,
                      std::shared_ptr<Aws::Http::HttpClient> http_client)
        : context(context), http_client(http_client) {}

private:
    AwsDiscordContextData context;
    std::shared_ptr<Aws::Http::HttpClient> http_client;
};

class AwsDiscordClient {
public:
    AwsDiscordClient(std::shared_ptr<Aws::Http::HttpClient> http)
        : http_client(http) {}

public:
    std::unique_ptr<AwsDiscordContext> create_context(
        AwsDiscordContextData data);

private:
    std::shared_ptr<Aws::Http::HttpClient> http_client;
};

}  // namespace loabot::discord