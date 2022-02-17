#pragma once

#include <gumbo.h>

#include <string>
#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace gumbo {

struct NodeView {
    template <typename _Selector>
    NodeView find(_Selector selector) const {
        auto result = selector(this->node);
        if (result == nullptr) {
            throw std::runtime_error("failed to find");
        }

        return { result };
    }

    NodeView operator [] (const std::size_t n) const {
        if (node->type != GUMBO_NODE_ELEMENT) {
            throw std::runtime_error("not a element type");
        }

        auto& children = node->v.element.children;
        if (children.length <= n) {
            throw std::runtime_error("child out of range");
        }

        using gumbo_size_t = decltype(children.length);
        std::size_t nth = n;
        for (gumbo_size_t i = 0; i < children.length; ++i) {
            if (static_cast<GumboNode*>(children.data[i])->type == GUMBO_NODE_ELEMENT) {
                if (nth == 0) {
                    return { static_cast<GumboNode*>(children.data[i]) };
                }

                nth--;
            }
        }

        throw std::runtime_error("element child out of range");
    }

    std::string direct_text() const;
    std::string full_text() const;

    std::size_t size() const;

    GumboNode* node;

public:
    void build_full_text(std::ostringstream& os, GumboNode* node) const;
};

std::string NodeView::direct_text() const {
    std::ostringstream oss;

    auto& children = node->v.element.children;
    using gumbo_size_t = decltype(children.length);
    for (gumbo_size_t i = 0; i < children.length; ++i) {
        GumboNode* child = (GumboNode*)(children.data[i]);
        if (child->type == GUMBO_NODE_TEXT) {
            oss << child->v.text.text;
        }
    }

    constexpr auto WHITE_SPACE = " \r\n\t\v";
    std::string text = oss.str();

    auto trim = [](auto&& str) {
        auto start = str.find_first_not_of(WHITE_SPACE);
        auto end = str.find_last_not_of(WHITE_SPACE);
        return str.substr(start, end + 1);
    };

    return trim(text);
}

std::string NodeView::full_text() const {
    std::ostringstream oss;

    build_full_text(oss, node);

    constexpr auto WHITE_SPACE = " \r\n\t\v";
    std::string text = oss.str();

    auto trim = [](auto&& str) {
        auto start = str.find_first_not_of(WHITE_SPACE);
        auto end = str.find_last_not_of(WHITE_SPACE);
        return str.substr(start, end + 1);
    };

    return trim(text);
}

void NodeView::build_full_text(std::ostringstream& oss, GumboNode* node) const
{
    if (node->type == GUMBO_NODE_TEXT) {
        oss << node->v.text.text;
    }
    else if (node->type == GUMBO_NODE_ELEMENT) {
        auto& children = node->v.element.children;
        using gumbo_size_t = decltype(children.length);
        for (gumbo_size_t i = 0; i < children.length; ++i) {
            build_full_text(oss, static_cast<GumboNode*>(children.data[i]));
        }
    }
}

std::size_t NodeView::size() const
{
    if (node->type != GUMBO_NODE_ELEMENT) {
        return 0;
    }

    auto& children = node->v.element.children;
    return std::count_if(children.data, children.data + children.length, [](void* ptr){
        auto child = static_cast<GumboNode*>(ptr);
        return child->type == GUMBO_NODE_ELEMENT;
    });
}

struct Root {
public:
    Root(const std::string& content)
        : gumbo(gumbo_parse(content.c_str()), &delete_gumbo) {}

public:
    NodeView root() const { return {gumbo->root}; }

private:
    static void delete_gumbo(GumboOutput* ptr) {
        gumbo_destroy_output(&kGumboDefaultOptions, ptr);
    }

    std::unique_ptr<GumboOutput, decltype(&delete_gumbo)> gumbo;
};

Root parse(const std::string& html) { return Root(html); }

namespace selector {

struct Id {
    std::string val;

    GumboNode* operator() (GumboNode* node) const {
        if (node->type != GUMBO_NODE_ELEMENT) {
            return nullptr;
        }

        auto id = gumbo_get_attribute(&(node->v.element.attributes), "id");

        if (id != nullptr && val == id->value) {
            return { node };
        }

        auto& children = node->v.element.children;
        using gumbo_size_t = decltype(children.length);
        for (gumbo_size_t i = 0; i < children.length; ++i) {
            auto matched = this->operator()((GumboNode*)(children.data[i]));
            if (matched != nullptr) {
                return matched;
            }
        }

        return { nullptr };
    }
};

struct Class {
    std::string val;

    GumboNode* operator() (GumboNode* node) const {
        if (node->type != GUMBO_NODE_ELEMENT) {
            return nullptr;
        }

        auto class_name = gumbo_get_attribute(&(node->v.element.attributes), "class");
        if (class_name != nullptr && val == class_name->value) {
            return { node };
        }

        auto& children = node->v.element.children;
        using gumbo_size_t = decltype(children.length);
        for (gumbo_size_t i = 0; i < children.length; ++i) {
            auto matched = this->operator()((GumboNode*)(children.data[i]));
            if (matched != nullptr) {
                return matched;
            }
        }

        return { nullptr };
    }
};

struct Tag {
    std::string val;

    GumboNode* operator() (GumboNode* node) const {
        if (node->type != GUMBO_NODE_ELEMENT) {
            return nullptr;
        }

        auto& elem = node->v.element;
        if (val == gumbo_normalized_tagname(elem.tag)) {
            return { node };
        }

        auto& children = node->v.element.children;
        using gumbo_size_t = decltype(children.length);
        for (gumbo_size_t i = 0; i < children.length; ++i) {
            auto matched = this->operator()((GumboNode*)(children.data[i]));
            if (matched != nullptr) {
                return matched;
            }
        }

        return { nullptr };
    }
};

struct Nth {
    std::size_t nth;
};

struct NthClass {
    std::string val;
    std::size_t nth;

    GumboNode* operator() (GumboNode* node) {
        if (node->type != GUMBO_NODE_ELEMENT) {
            return nullptr;
        }

        auto class_name = gumbo_get_attribute(&(node->v.element.attributes), "class");
        if (class_name != nullptr && val == class_name->value) {
            if (nth == 0) {
                return { node };
            }

            nth--;
        }

        auto& children = node->v.element.children;
        using gumbo_size_t = decltype(children.length);
        for (gumbo_size_t i = 0; i < children.length; ++i) {
            auto matched = this->operator()((GumboNode*)(children.data[i]));
            if (matched != nullptr) {
                return matched;
            }
        }

        return { nullptr };
    }
};

NthClass operator&&(const Class& id, const Nth& nth) {
    return {id.val, nth.nth};
}

struct NthTag {
    std::string val;
    std::size_t nth;

    GumboNode* operator() (GumboNode* node) {
        if (node->type != GUMBO_NODE_ELEMENT) {
            return nullptr;
        }

        auto& elem = node->v.element;

        if (val == gumbo_normalized_tagname(elem.tag)) {
            if (nth == 0) {
                return { node };
            }

            nth--;
        }

        auto& children = node->v.element.children;
        using gumbo_size_t = decltype(children.length);
        for (gumbo_size_t i = 0; i < children.length; ++i) {
            auto matched = this->operator()((GumboNode*)(children.data[i]));
            if (matched != nullptr) {
                return matched;
            }
        }

        return { nullptr };
    }
};

NthTag operator&&(const Tag& tag, const Nth& nth) { return {tag.val, nth.nth}; }

}  // namespace selector

}  // namespace gumbo