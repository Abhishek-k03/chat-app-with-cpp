#pragma once

#include <queue>
#include <mutex>
#include "Message.h"

class MessageQueue
{
private:
    std::queue<Message> messages;
    std::mutex mtx;
    bool shutdown = false;

public:
    void push(const Message &message);
    bool pop(Message &message);
    void shutdownQueue();
};
