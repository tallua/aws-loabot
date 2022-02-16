#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "./lostark_data.hpp"

namespace loabot::data::fetch {

class DataFetcher {
public:
public:
    virtual const CharacterData& FetchCharacter() = 0;
    virtual const StatData& FetchStat() = 0;
    virtual const MokokoData& FetchMokoko() = 0;
};

class LoaHomepageDataFetcher : public DataFetcher {
public:
    const CharacterData& FetchCharacter() override;
    const StatData& FetchStat() override;
    const MokokoData& FetchMokoko() override;

private:
    struct Context;
    std::unique_ptr<Context> context;

    std::optional<CharacterData> character;
    std::optional<StatData> stat;
    std::optional<MokokoData> mokoko;
};

}  // namespace loabot::data::fetch