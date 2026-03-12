#pragma once

#include <atomic>
#include "Platform.h"
#include "ClientRegistry.h"
#include "History.h"
#include "UndoRedo.h"
#include "MessageQueue.h"
#include "ChatServices.h"
#include "CommandRouter.h"
#include "Broadcaster.h"

constexpr int DEFAULT_PORT = 8080;

class ChatServer
{
private:
    int port;
    SOCKET listenSocket;
    bool initialized;

    ClientRegistry registry;
    History history;
    UndoRedo undoRedo;
    MessageQueue queue;
    std::atomic<int> counter;
    ChatServices services;
    CommandRouter router;
    Broadcaster broadcaster;

    void handleConnection(SOCKET clientSocket);

public:
    explicit ChatServer(int port = DEFAULT_PORT);
    ~ChatServer();

    bool start();
    void run();
};
