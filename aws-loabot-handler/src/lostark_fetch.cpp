#include "loabot/loabot_fetch.hpp"

#include <gumbo.h>

namespace loabot::data::fetch {

struct LoaHomepageDataFetcher::Context {



};

const CharacterData& LoaHomepageDataFetcher::FetchCharacter() {
    if (!character.has_value()) {
        //
    }

    return character.value();
}

const StatData& LoaHomepageDataFetcher::FetchStat() {
    if (!stat.has_value()) {
        //
    }

    return stat.value();
}

const MokokoData& LoaHomepageDataFetcher::FetchMokoko() {
    if (!mokoko.has_value()) {
        //
    }

    return mokoko.value();
}

}  // namespace loabot::data::fetch