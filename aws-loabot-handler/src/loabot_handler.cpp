#include "loabot/handler.hpp"

#include "loabot/log.hpp"
#include "loabot/http.hpp"

#include <aws/core/http/HttpResponse.h>

using namespace loabot::log;
using namespace loabot::handler;
using namespace loabot::http;

using namespace Aws::Utils::Json;
using namespace Aws::Http;

namespace
{

JsonView find_option(const Aws::Utils::Array<JsonView>& options, const std::string& name)
{
    for(auto i = std::size_t{0}; i < options.GetLength(); ++i) {
        if (options.GetItem(i).GetString("name") == name) {
            return options.GetItem(i);
        }
    }

    throw std::out_of_range("name not exist: " + name);
}

}

namespace lostark::http
{

std::string get_character_page(std::shared_ptr<HttpClient> http_client, const std::string& name)
{
    auto http_request = create_request({
        "https://m-lostark.game.onstove.com/Profile/Character/" + name,
        Aws::Http::HttpMethod::HTTP_GET,
        {},
        {},
        {}
    });

    const auto http_response = http_client->MakeRequest(http_request);
    const auto http_response_code = http_response->GetResponseCode();
    if (http_response_code != HttpResponseCode::OK) {
        LOG("Error: http status code(", int(http_response->GetResponseCode()), ")");
        throw std::runtime_error("invalid response status code: " + std::to_string(int(http_response_code)));
    }

    const auto http_response_body = [&]{
        std::stringstream ss;
        ss << http_response->GetResponseBody().rdbuf();
        return ss.str();
    }();
    LOG("Response Code: (", int(http_response->GetResponseCode()), ") : Length: ", http_response_body.size());
    return http_response_body;
}

}

std::unique_ptr<CommandHandler> LoabotHandlerBuilder::build(std::shared_ptr<Aws::Http::HttpClient> http_client)
{
    auto handler = std::make_unique<CommandHandler>();

    handler->add_handler("캐릭터", [http_client] (JsonView view) {
        const auto character_name = find_option(view.GetArray("options"), "이름").GetString("value");
        const auto character_page = lostark::http::get_character_page(http_client, character_name);


        return JsonValue(R"({ "content": "조금만 기다려주세요..." })");
    });
    handler->add_handler("거래소", [http_client] (JsonView) {
        return JsonValue(R"({ "content": "아직 기능 구현중이에요..." })");
    });
    handler->add_handler("수집품", [http_client] (JsonView) {
        return JsonValue(R"({ "content": "조금만 기다려주세요..." })");
    });

    return handler;
}
