#include "loabot/handler.hpp"

using namespace loabot::handler;

std::unique_ptr<CommandHandler> LoabotHandlerBuilder::build()
{
    auto handler = std::make_unique<CommandHandler>();

    return handler;
}
