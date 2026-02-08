#include "Utils.h"
#include <ctime>

namespace util
{
    std::string currentTime()
    {
        time_t now = time(nullptr);
        tm localTime;
        localtime_s(&localTime, &now);

        char buffer[20];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", &localTime);
        return std::string(buffer);
    }

    std::string formatMessage(const std::string &sender, const std::string &text)
    {
        return "[" + currentTime() + "][" + sender + "]: " + text;
    }

    void sendLine(SOCKET socket, const std::string &message)
    {
        send(socket, message.c_str(), (int)message.size(), 0);
    }

    std::string trim(const std::string &text)
    {
        std::string result = text;
        while (!result.empty() && result.front() == ' ')
            result.erase(result.begin());
        while (!result.empty() && result.back() == ' ')
            result.pop_back();
        return result;
    }

    bool splitFirst(const std::string &text, std::string &first, std::string &rest)
    {
        size_t space = text.find(' ');
        if (space == std::string::npos || space + 1 >= text.size())
            return false;
        first = text.substr(0, space);
        rest = text.substr(space + 1);
        return true;
    }
}
