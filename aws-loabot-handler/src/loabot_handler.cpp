#include "loabot/loabot_handler.hpp"

#include <aws/core/http/HttpResponse.h>

#include <algorithm>
#include <array>

#include "loabot/loabot_fetch.hpp"
#include "loabot/loabot_http.hpp"
#include "loabot/loabot_response.hpp"
#include "loabot/log.hpp"

using namespace loabot::log;
using namespace loabot::handler;
using namespace loabot::http;

using namespace Aws::Utils::Json;
using namespace Aws::Http;

namespace {

JsonView find_option(const Aws::Utils::Array<JsonView>& options,
                     const std::string& name) {
    for (auto i = std::size_t{0}; i < options.GetLength(); ++i) {
        if (options.GetItem(i).GetString("name") == name) {
            return options.GetItem(i);
        }
    }

    throw std::out_of_range("name not exist: " + name);
}

using namespace std::literals::string_literals;
std::array mokoko_alias{"모코코의 씨앗"s, "모코코"s};
std::array island_alias{"섬의 마음"s, "섬마"s};
std::array tome_alias{"이그네아의 징표"s, "이그네아"s, "징표"s};
std::array star_alias{"오르페우스의 별"s, "별"s};
std::array giant_alias{"거인의 심장"s, "심장"s};
std::array masterpiece_alias{"위대한 미술품"s, "미술품"s};
std::array bounties_alias{"항해 모험물"s, "모험물"s};
std::array leaf_alias{"세계수의 잎"s, "세계수"s};

}  // namespace

LoabotBuilder::HandlerPtr LoabotBuilder::build(
    std::shared_ptr<Aws::Http::HttpClient> http_client) {
    using loabot::data::fetch::LoaHomepageDataFetcher;

    auto handler = std::make_unique<Handler>();

    handler->add_command("캐릭터", [http_client](const Handler::Command& command) {
        const auto character_name =
            find_option(command.payload.GetArray("options"), "이름")
                .GetString("value");

        auto character_fetcher =
            std::make_unique<LoaHomepageDataFetcher>(character_name, http_client);

        const auto& character = character_fetcher->fetch_character();
        const auto& stat = character_fetcher->fetch_stat();

        return response::format_character(character, stat);
    });

    handler->add_command("수집품", [http_client](const Handler::Command& command) {
        const auto character_name =
            find_option(command.payload.GetArray("options"), "이름")
                .GetString("value");

        auto character_fetcher =
            std::make_unique<LoaHomepageDataFetcher>(character_name, http_client);

        const auto collection_name =
            find_option(command.payload.GetArray("options"), "종류")
                .GetString("value");
        auto match_collection = [&](auto&& aliases) {
            return std::find(aliases.begin(), aliases.end(), collection_name) !=
                   aliases.end();
        };

        if (match_collection(mokoko_alias)) {
            const auto& mokoko = character_fetcher->fetch_mokoko_seeds();

            return response::format_mokoko(mokoko);
        }

        if (match_collection(island_alias)) {
            const auto& island = character_fetcher->fetch_island_heart();

            return response::format_collection(island);
        }

        if (match_collection(tome_alias)) {
            const auto& tome = character_fetcher->fetch_ignea_token();

            return response::format_collection(tome);
        }

        if (match_collection(star_alias)) {
            const auto& star = character_fetcher->fetch_orpheus_star();

            return response::format_collection(star);
        }

        if (match_collection(giant_alias)) {
            const auto& giant = character_fetcher->fetch_giants_heart();

            return response::format_collection(giant);
        }

        if (match_collection(masterpiece_alias)) {
            const auto& masterpiece = character_fetcher->fetch_masterpiece();

            return response::format_collection(masterpiece);
        }

        if (match_collection(bounties_alias)) {
            const auto& bounties = character_fetcher->fetch_sea_bounties();

            return response::format_collection(bounties);
        }

        if (match_collection(leaf_alias)) {
            const auto& leaves = character_fetcher->fetch_worldtree_leaf();

            return response::format_collection(leaves);
        }

        return discord::message::Content{collection_name + "이 무엇인가요?"};
    });

    return handler;
}
