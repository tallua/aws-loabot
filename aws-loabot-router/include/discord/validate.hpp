#pragma once

#include <aws/core/utils/json/JsonSerializer.h>

namespace discord {

bool validate_message(const Aws::Utils::Json::JsonView& req);

}