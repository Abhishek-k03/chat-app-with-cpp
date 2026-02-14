#include "MessageQueue.h"

void MessageQueue::push(const Message &message)
{
    std::lock_guard<std::mutex> lock(mtx);
    messages.push(message);
}

bool MessageQueue::pop(Message &message)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (messages.empty() || shutdown)
        return false;

    message = messages.front();
    messages.pop();
    return true;
}

void MessageQueue::shutdownQueue()
{
    std::lock_guard<std::mutex> lock(mtx);
    shutdown = true;
}
