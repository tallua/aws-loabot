#include "loabot/loabot_handler.hpp"

#include <aws/core/http/HttpResponse.h>

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

}  // namespace

std::unique_ptr<discord::handler::DiscordHandler> LoabotBuilder::build(
    Aws::Client::ClientConfiguration config) {
    using discord::handler::Command;
    using discord::message::Content;
    using discord::message::Embed;
    using loabot::data::fetch::LoaHomepageDataFetcher;

    auto handler = std::make_unique<discord::handler::DiscordHandler>();

    handler->add_command("캐릭터", [](const Command& command) {
        const auto character_name =
            find_option(command.payload.GetArray("options"), "이름")
                .GetString("value");

        auto fetcher = std::make_unique<LoaHomepageDataFetcher>();

        const auto& character = fetcher->FetchCharacter();
        const auto& stat = fetcher->FetchStat();

        return response::format_character(character, stat);
    });
    handler->add_command("수집품", [](const Command& command) {
        const auto character_name =
            find_option(command.payload.GetArray("options"), "이름")
                .GetString("value");
        const auto collection_name =
            find_option(command.payload.GetArray("options"), "종류")
                .GetString("value");

        return Content{"조금만 기다려주세요..."};
    });

    return handler;
}
