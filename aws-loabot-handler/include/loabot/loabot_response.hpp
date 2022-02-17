#pragma once

#include <discord/discord_message.hpp>

#include "loabot_data.hpp"

namespace loabot::response {

using discord::message::Content;
using discord::message::Embed;

discord::message::Content format_character(data::CharacterData, data::StatData) {
    // TODO
    return {""};
}
discord::message::Content format_mokoko(data::MokokoData) {
    // TODO
    return {""};
}
discord::message::Content format_collection(data::CollectionData) {
    // TODO
    return {""};
}

}