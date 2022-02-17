#pragma once

#include <discord/discord_message.hpp>

#include "loabot_data.hpp"

namespace loabot::response {

using discord::message::Content;
using discord::message::Embed;

discord::message::Embed format_character(const data::CharacterData&, const data::StatData&);
discord::message::Content format_mokoko(data::MokokoData) {
    // TODO
    return {"아직 준비중이에요"};
}
discord::message::Content format_collection(data::CollectionData) {
    // TODO
    return {"아직 준비중이에요"};
}

}