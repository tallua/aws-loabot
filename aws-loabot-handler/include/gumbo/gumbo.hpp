#pragma once

#include <string>

namespace gumbo {

struct Node {
    template <typename _Tp>
    Node find(_Tp matcher) {
        return matcher(*this);
    }

    std::string direct_text() const;
    std::string full_text() const;
};

struct Root {
    Node& root();
};

Root parse(const std::string& html);

namespace selector {

struct Id {
    std::string val;
};

struct Class {
    std::string val;
};

struct Tag {
    std::string val;
};

struct Nth {
    std::size_t nth;
};

struct NthClass {
    std::string val;
    std::size_t nth;
};

NthClass operator&&(const Class& id, const Nth& nth) {
    return {id.val, nth.nth};
}

struct NthTag {
    std::string val;
    std::size_t nth;
};

NthTag operator&&(const Tag& tag, const Nth& nth) { return {tag.val, nth.nth}; }

}  // namespace selector

}  // namespace gumbo