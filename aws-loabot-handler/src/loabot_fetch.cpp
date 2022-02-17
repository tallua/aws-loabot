#include "loabot/loabot_fetch.hpp"

#include <gumbo.h>

namespace {


    
}  // namespace

namespace loabot::data::fetch {

class LoaHomepageDataFetcher::Context {
public:
    Context(const std::string& name) : name(name) {}

public:
    CharacterData get_character();
    StatData get_stat();
    MokokoData get_mokoko(const std::string& tab_id);
    CollectionData get_collection(const std::string& tab_id);

private:
    std::string name;
    std::optional<std::string> character_page;
    std::optional<std::string> collection_page;
};

LoaHomepageDataFetcher::LoaHomepageDataFetcher(const std::string& name)
    : context(std::make_unique<Context>(name)) {}

LoaHomepageDataFetcher::~LoaHomepageDataFetcher() = default;

const CharacterData& LoaHomepageDataFetcher::fetch_character() {
    if (!character.has_value()) {
        character = context->get_character();
    }

    return character.value();
}

const StatData& LoaHomepageDataFetcher::fetch_stat() {
    if (!stat.has_value()) {
        stat = context->get_stat();
    }

    return stat.value();
}

const MokokoData& LoaHomepageDataFetcher::fetch_mokoko_seeds() {
    if (!mokoko_seeds.has_value()) {
        mokoko_seeds = context->get_mokoko("lui-tab1-5");
    }

    return mokoko_seeds.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_island_heart() {
    if (!island_heart.has_value()) {
        island_heart = context->get_collection("lui-tab1-1");
    }

    return island_heart.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_ignea_token() {
    if (!ignea_token.has_value()) {
        ignea_token = context->get_collection("lui-tab1-7");
    }

    return ignea_token.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_orpheus_star() {
    if (!orpheus_star.has_value()) {
        orpheus_star = context->get_collection("lui-tab1-2");
    }

    return orpheus_star.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_giants_heart() {
    if (!giants_heart.has_value()) {
        giants_heart = context->get_collection("lui-tab1-3");
    }

    return giants_heart.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_masterpiece() {
    if (!masterpiece.has_value()) {
        masterpiece = context->get_collection("lui-tab1-4");
    }

    return masterpiece.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_sea_bounties() {
    if (!sea_bounties.has_value()) {
        sea_bounties = context->get_collection("lui-tab1-6");
    }

    return sea_bounties.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_worldtree_leaf() {
    if (!worldtree_leaf.has_value()) {
        worldtree_leaf = context->get_collection("lui-tab1-8");
    }

    return worldtree_leaf.value();
}

}  // namespace loabot::data::fetch