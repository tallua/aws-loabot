#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "loabot/loabot_data.hpp"

namespace loabot::data::fetch {

class DataFetcher {
public:
public:
    virtual const CharacterData& fetch_character() = 0;
    virtual const StatData& fetch_stat() = 0;
    virtual const MokokoData& fetch_mokoko_seeds() = 0;
    virtual const CollectionData& fetch_island_heart() = 0;
    virtual const CollectionData& fetch_ignea_token() = 0;
    virtual const CollectionData& fetch_orpheus_star() = 0;
    virtual const CollectionData& fetch_giants_heart() = 0;
    virtual const CollectionData& fetch_masterpiece() = 0;
    virtual const CollectionData& fetch_sea_bounties() = 0;
    virtual const CollectionData& fetch_worldtree_leaf() = 0;
};

class LoaHomepageDataFetcher : public DataFetcher {
public:
    LoaHomepageDataFetcher(const std::string& name);
    ~LoaHomepageDataFetcher();

public:
    const CharacterData& fetch_character() override;
    const StatData& fetch_stat() override;
    const MokokoData& fetch_mokoko_seeds() override;
    const CollectionData& fetch_island_heart() override;
    const CollectionData& fetch_ignea_token() override;
    const CollectionData& fetch_orpheus_star() override;
    const CollectionData& fetch_giants_heart() override;
    const CollectionData& fetch_masterpiece() override;
    const CollectionData& fetch_sea_bounties() override;
    const CollectionData& fetch_worldtree_leaf() override;

private:
    class Context;
    std::unique_ptr<Context> context;

    std::optional<CharacterData> character;
    std::optional<StatData> stat;
    std::optional<MokokoData> mokoko_seeds;
    std::optional<CollectionData> island_heart;
    std::optional<CollectionData> ignea_token;
    std::optional<CollectionData> orpheus_star;
    std::optional<CollectionData> giants_heart;
    std::optional<CollectionData> masterpiece;
    std::optional<CollectionData> sea_bounties;
    std::optional<CollectionData> worldtree_leaf;
};

}  // namespace loabot::data::fetch