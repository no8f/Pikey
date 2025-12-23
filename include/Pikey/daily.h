#pragma once

#include <dpp/dpp.h>

namespace Pikey {
    void sendDaily(dpp::cluster* bot);

    [[noreturn]] void setupDaily(int target_hour, int target_minute, dpp::cluster* bot);
}