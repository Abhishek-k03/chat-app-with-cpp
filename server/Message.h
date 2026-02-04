#pragma once

#include <string>
#include <ctime>

class Message
{
public:
    int id;
    std::string sender;
    std::string text;
    time_t timestamp;

    Message(int id = 0, std::string sender = "", std::string text = "");

    std::string toString() const;
};
