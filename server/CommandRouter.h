#pragma once

#include <map>
#include <memory>
#include <string>
#include "Command.h"

class CommandRouter
{
private:
    std::map<std::string, std::unique_ptr<ICommand>> commands;

public:
    CommandRouter();

    bool dispatch(const std::string &input, ClientSession &session, ChatServices &services);
};
