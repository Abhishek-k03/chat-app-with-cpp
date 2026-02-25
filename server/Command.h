#pragma once

#include <string>
#include "Platform.h"
#include "ChatServices.h"

struct ClientSession
{
    SOCKET socket;
    std::string username;
    std::string currentRoom;
};

struct CommandContext
{
    ClientSession &session;
    std::string args;
    ChatServices &services;
};

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual void execute(CommandContext &context) = 0;
};
