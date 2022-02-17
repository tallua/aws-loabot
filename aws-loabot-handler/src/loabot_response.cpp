#include "loabot/loabot_response.hpp"

#include <algorithm>

namespace loabot::response {

discord::message::Embed format_character(const data::CharacterData& character,
                                         const data::StatData& stat) {
    using namespace std::literals::string_literals;

    discord::message::Embed embed;

    embed.title = character.name;
    embed.description = character.job + "@" + character.server;

    embed.fields.push_back({"아이템 레벨"s, stat.item_level});
    embed.fields.push_back({"원정대 레벨"s, stat.expedition_level});
    embed.fields.push_back({"공격력"s, stat.attack_point});
    embed.fields.push_back({"체력"s, stat.health_point});

    embed.fields.push_back({""s, ""s, false});

    auto combat_stats = stat.combat_stats;
    std::sort(combat_stats.begin(), combat_stats.end(), []() {

    });
    embed.fields.push_back(
        {combat_stats[0].name, std::to_string(combat_stats[0].value)});
    embed.fields.push_back(
        {combat_stats[1].name, std::to_string(combat_stats[1].value)});

    embed.fields.push_back({""s, ""s, false});

    std::for_each(stat.engraves.begin(), stat.engraves.end(),
                  [&](auto&& engrave) {
                      embed.fields.push_back(
                          {engrave.name, std::to_string(engrave.level)});
                  });

    return embed;
}

}  // namespace loabot::response