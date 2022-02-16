#pragma once

#include <optional>
#include <string>
#include <vector>

namespace discord::message {

struct Content {
    std::string data;
};

struct Embed {
    std::optional<std::string> title;
    std::optional<std::string> title;
};

}  // namespace discord::message