#include "loabot/loabot_handler.hpp"

#include "loabot/log.hpp"
#include "loabot/loabot_http.hpp"
#include "loabot/loabot_fetch.hpp"

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

std::unique_ptr<discord::handler::DiscordHandler> LoabotBuilder::build(Aws::Client::ClientConfiguration config)
{
    using loabot::data::fetch::LoaHomepageDataFetcher;
    using discord::message::Content;
    using discord::message::Embed;
    using discord::handler::Command;

    auto handler = std::make_unique<discord::handler::DiscordHandler>();

    handler->add_command("캐릭터", [] (Command command) {
        const auto character_name = find_option(command.payload.GetArray("options"), "이름").GetString("value");
        
        auto fetcher = std::make_unique<LoaHomepageDataFetcher>();

        auto character = fetcher->FetchCharacter();
        auto stat = fetcher->FetchStat();


        return Content{ "조금만 기다려주세요..." };
    });
    handler->add_command("거래소", [] (Command) {
        return Content{ "조금만 기다려주세요..." };
    });
    handler->add_command("수집품", [] (Command) {
        return Content{ "조금만 기다려주세요..." };
    });

    return handler;
}
