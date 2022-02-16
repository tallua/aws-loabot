#pragma once

#include <string>

namespace loabot::data {

struct CharacterData {

};

struct StatData {

};

template <typename T>
struct CollectionItem {
    std::string name;
    std::string category;
    T data;
};

struct MokokoCount {
    std::size_t collected;
    std::size_t total;
};

using MokokoData = std::vector<CollectionItem<MokokoCount>>;

}