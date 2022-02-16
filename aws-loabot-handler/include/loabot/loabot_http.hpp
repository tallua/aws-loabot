#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <sstream>

#include <aws/core/http/URI.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/http/HttpRequest.h>

#include <aws/core/utils/json/JsonSerializer.h>

namespace loabot::http {

struct WebRequestParam {
    struct HeaderData {
        std::string key;
        std::string value;
    };
    Aws::Http::URI uri;
    Aws::Http::HttpMethod method;
    std::vector<HeaderData> headers;

    std::optional<std::string> authorization;
    std::optional<Aws::Utils::Json::JsonValue> body;
};

inline std::shared_ptr<Aws::Http::HttpRequest> create_request(const WebRequestParam& param)
{
    const auto request = Aws::Http::CreateHttpRequest(param.uri, param.method, []() {
        return new std::stringstream();
    });

    std::for_each(param.headers.begin(), param.headers.end(), [&](auto&& data) {
        request->SetHeaderValue(data.key, data.value);
    });

    if (param.body.has_value()) {
        auto body_string = param.body.value().View().WriteCompact();
        request->SetContentLength(std::to_string(body_string.size()));
        request->AddContentBody(std::make_shared<std::stringstream>(std::move(body_string)));
        request->SetContentType("application/json");
    }

    if (param.authorization.has_value()) {
        request->SetAuthorization(param.authorization.value());
    }

    return request;
}

}