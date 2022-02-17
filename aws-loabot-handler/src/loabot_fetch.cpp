#include "loabot/loabot_fetch.hpp"

#include <gumbo.h>

namespace {

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
namespace loabot::data::fetch {

struct LoaHomepageDataFetcher::Context {

CharacterData get_character();


};

LoaHomepageDataFetcher::LoaHomepageDataFetcher() = default;
LoaHomepageDataFetcher::~LoaHomepageDataFetcher() = default;

const CharacterData& LoaHomepageDataFetcher::fetch_character() {
    if (!character.has_value()) {
        //
    }

    return character.value();
}

const StatData& LoaHomepageDataFetcher::fetch_stat() {
    if (!stat.has_value()) {
        //
    }

    return stat.value();
}

const MokokoData& LoaHomepageDataFetcher::fetch_mokoko_seeds() {
    if (!mokoko.has_value()) {
        //
    }

    return mokoko.value();
}

}  // namespace loabot::data::fetch