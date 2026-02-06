#pragma once

#include <string>
#include "Platform.h"

namespace util
{
    std::string currentTime();
    std::string formatMessage(const std::string &sender, const std::string &text);
    void sendLine(SOCKET socket, const std::string &message);
    std::string trim(const std::string &text);
    bool splitFirst(const std::string &text, std::string &first, std::string &rest);
}
