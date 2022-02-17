#pragma once

#include <optional>
#include <string>
#include <vector>

namespace discord::message {

struct Content {
    std::string data;
};

struct Embed {
    struct Field {
        std::string name;
        std::string value;
        bool inlined = true;
    };

    std::optional<std::string> title;
    std::optional<std::string> description;
    std::optional<std::string> url;
    std::optional<std::string> thumbnail;
    std::vector<Field> fields;
};

}  // namespace discord::message