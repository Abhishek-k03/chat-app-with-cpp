#pragma once

#include <string>
#include "Platform.h"
#include "Command.h"

class CommandRouter;

class ClientHandler
{
private:
    SOCKET socket;
    ChatServices &services;
    CommandRouter &router;

    bool readUsername(std::string &username);
    void postMessage(const ClientSession &session, const std::string &text);

public:
    ClientHandler(SOCKET socket, ChatServices &services, CommandRouter &router);

    void run();
};
