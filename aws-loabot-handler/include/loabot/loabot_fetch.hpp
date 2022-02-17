#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "loabot/loabot_data.hpp"

namespace loabot::data::fetch {

class DataFetcher {
public:
public:
    virtual const CharacterData& FetchCharacter() = 0;
    virtual const StatData& FetchStat() = 0;
    virtual const MokokoData& FetchMokoko() = 0;
    virtual const CollectionData& FetchIslandHeart() = 0;
    virtual const CollectionData& FetchIgneaToken() = 0;
    virtual const CollectionData& FetchOrpheusStar() = 0;
    virtual const CollectionData& FetchGiantHeart() = 0;
    virtual const CollectionData& FetchMasterpiece() = 0;
    virtual const CollectionData& FetchSeaBounties() = 0;
    virtual const CollectionData& FetchWorldTreeLeaf() = 0;
};

class LoaHomepageDataFetcher : public DataFetcher {
public:
    LoaHomepageDataFetcher(const std::string& name);
    ~LoaHomepageDataFetcher();

public:
    const CharacterData& FetchCharacter() override;
    const StatData& FetchStat() override;
    const MokokoData& FetchMokoko() override;
    const CollectionData& FetchIslandHeart() override;
    const CollectionData& FetchIgneaToken() override;
    const CollectionData& FetchOrpheusStar() override;
    const CollectionData& FetchGiantHeart() override;
    const CollectionData& FetchMasterpiece() override;
    const CollectionData& FetchSeaBounties() override;
    const CollectionData& FetchWorldTreeLeaf() override;

private:
    struct Context;
    std::unique_ptr<Context> context;

    std::optional<CharacterData> character;
    std::optional<StatData> stat;
    std::optional<MokokoData> mokoko;
};

}  // namespace loabot::data::fetch