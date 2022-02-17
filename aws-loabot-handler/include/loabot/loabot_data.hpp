#pragma once

#include <string>

namespace loabot::data {

struct CharacterData {
    std::string name;
    std::string job;
    std::string server;

    std::string thumbnail;
    std::string url;
};

struct StatData {
    struct Combat {
        std::string name;
        int value;
    };

    struct Engrave {
        std::string name;
        int level;
    };

    std::string item_level;
    std::string expedition_level;
    std::string health_point;
    std::string attack_point;
    std::vector<Combat> combat_stats;
    std::vector<Engrave> engraves;
};

template <typename T>
struct CollectionItem {
    std::string name;
    T data;
};

struct MokokoCount {
    std::size_t collected;
    std::size_t total;
};

using MokokoData = std::vector<CollectionItem<MokokoCount>>;
using CollectionData = std::vector<CollectionItem<bool>>;

}