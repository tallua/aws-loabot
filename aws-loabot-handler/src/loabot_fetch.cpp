#include "loabot/loabot_fetch.hpp"

#include "gumbo/gumbo.hpp"

#include "loabot/log.hpp"

using namespace loabot::log;

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
    const std::string& get_main_page();
    const std::string& get_collection_page();

private:
    std::string name;
    std::optional<std::string> cached_main_page;
    std::optional<std::string> cached_collection_page;
};

LoaHomepageDataFetcher::LoaHomepageDataFetcher(const std::string& name)
    : context(std::make_unique<Context>(name)) {}

LoaHomepageDataFetcher::~LoaHomepageDataFetcher() = default;

const CharacterData& LoaHomepageDataFetcher::fetch_character() {
    if (!character.has_value()) {
        LOG("Fetching character data");
        character = context->get_character();
    }

    return character.value();
}

const StatData& LoaHomepageDataFetcher::fetch_stat() {
    if (!stat.has_value()) {
        LOG("Fetching stat data");
        stat = context->get_stat();
    }

    return stat.value();
}

const MokokoData& LoaHomepageDataFetcher::fetch_mokoko_seeds() {
    if (!mokoko_seeds.has_value()) {
        LOG("Fetching mokoko seeds");
        mokoko_seeds = context->get_mokoko("lui-tab1-5");
    }

    return mokoko_seeds.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_island_heart() {
    if (!island_heart.has_value()) {
        LOG("Fetching island heart");
        island_heart = context->get_collection("lui-tab1-1");
    }

    return island_heart.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_ignea_token() {
    if (!ignea_token.has_value()) {
        LOG("Fetching ignea token");
        ignea_token = context->get_collection("lui-tab1-7");
    }

    return ignea_token.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_orpheus_star() {
    if (!orpheus_star.has_value()) {
        LOG("Fetching orpheus star");
        orpheus_star = context->get_collection("lui-tab1-2");
    }

    return orpheus_star.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_giants_heart() {
    if (!giants_heart.has_value()) {
        LOG("Fetching giant's heart");
        giants_heart = context->get_collection("lui-tab1-3");
    }

    return giants_heart.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_masterpiece() {
    if (!masterpiece.has_value()) {
        LOG("Fetching masterpiece");
        masterpiece = context->get_collection("lui-tab1-4");
    }

    return masterpiece.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_sea_bounties() {
    if (!sea_bounties.has_value()) {
        LOG("Fetching sea bounties");
        sea_bounties = context->get_collection("lui-tab1-6");
    }

    return sea_bounties.value();
}

const CollectionData& LoaHomepageDataFetcher::fetch_worldtree_leaf() {
    if (!worldtree_leaf.has_value()) {
        LOG("Fetching worldtree leaf");
        worldtree_leaf = context->get_collection("lui-tab1-8");
    }

    return worldtree_leaf.value();
}

CharacterData LoaHomepageDataFetcher::Context::get_character() {
    using gumbo::selector::Class;
    using gumbo::selector::Id;
    using gumbo::selector::Nth;
    using gumbo::selector::Tag;

    const auto& main_page = get_main_page();

    auto character_node =
        gumbo::parse(main_page).root().find(Class{"myinfo__character"});

    CharacterData data;
    data.name =
        character_node.find(Id{"myinfo__character--button2"}).direct_text();
    data.server = character_node.find(Class{"wrapper-define"})
                      .find(Tag{"dl"} && Nth{0})
                      .find(Tag{"dd"})
                      .direct_text()
                      .substr(1);
    data.job = character_node.find(Class{"wrapper-define"})
                   .find(Tag{"dl"} && Nth{1})
                   .find(Tag{"dd"})
                   .direct_text();

    return data;
}

StatData LoaHomepageDataFetcher::Context::get_stat() {
    using gumbo::selector::Class;
    using gumbo::selector::Id;
    using gumbo::selector::Nth;
    using gumbo::selector::Tag;

    const auto& main_page = get_main_page();

    auto character_node =
        gumbo::parse(main_page).root().find(Id{"profile-char"});

    StatData data;
    auto level_node =
        character_node.find(Class{"profile-ability-basic"} && Nth{0})
            .find(Tag{"ul"});
    data.attack_point = level_node.find(Tag{"li"} && Nth{0})
                            .find(Tag{"span"} && Nth{1})
                            .direct_text();
    data.health_point = level_node.find(Tag{"li"} && Nth{1})
                            .find(Tag{"span"} && Nth{1})
                            .direct_text();

    auto stat_node =
        character_node.find(Class{"profile-ability-basic"} && Nth{1})
            .find(Tag{"ul"});
    auto engrave_node =
        character_node.find(Class{"profile-ability-engrave"}).find(Tag{"ul"});

    return data;
}

MokokoData LoaHomepageDataFetcher::Context::get_mokoko(
    const std::string& tab_id) {
    using gumbo::selector::Class;
    using gumbo::selector::Id;
    using gumbo::selector::Tag;

    const auto& collection_page = get_collection_page();
    auto list = gumbo::parse(collection_page)
                    .root()
                    .find(Id{tab_id})
                    .find(Class{"list"})
                    .find(Tag{"li"});

    MokokoData data;

    return data;
}

CollectionData LoaHomepageDataFetcher::Context::get_collection(
    const std::string& tab_id) {
    using gumbo::selector::Class;
    using gumbo::selector::Id;
    using gumbo::selector::Tag;

    const auto& collection_page = get_collection_page();
    auto list = gumbo::parse(collection_page)
                    .root()
                    .find(Id{tab_id})
                    .find(Class{"list"})
                    .find(Tag{"li"});
    CollectionData data;

    return data;
}

const std::string& LoaHomepageDataFetcher::Context::get_main_page() {
    if (!cached_main_page.has_value()) {
        
    }

    return cached_main_page.value();
}

const std::string& LoaHomepageDataFetcher::Context::get_collection_page() {
    if (!cached_collection_page.has_value()) {
        // TODO
        throw std::exception("not implemented getting collection page");
    }

    return cached_collection_page.value();
}
}  // namespace loabot::data::fetch
