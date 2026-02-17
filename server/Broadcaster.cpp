#include "Broadcaster.h"
#include "ClientRegistry.h"
#include "MessageQueue.h"
#include "Message.h"
#include "Utils.h"
#include <chrono>

Broadcaster::Broadcaster(ClientRegistry &registry, MessageQueue &queue)
    : registry(registry), queue(queue) {}

Broadcaster::~Broadcaster()
{
    stop();
}

void Broadcaster::start()
{
    running = true;
    worker = std::thread(&Broadcaster::run, this);
}

void Broadcaster::stop()
{
    running = false;
    queue.shutdownQueue();
    if (worker.joinable())
        worker.join();
}

void Broadcaster::run()
{
    while (running)
    {
        Message message;
        if (!queue.pop(message))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        std::string fullMessage = message.toString() + "\n";
        std::string senderMessage = "[" + util::currentTime() + "] " + "\n";

        std::string room = registry.roomOfUser(message.sender);
        if (room.empty())
            continue;

        for (const auto &member : registry.snapshotRoom(room))
        {
            if (member.second == message.sender)
                util::sendLine(member.first, senderMessage);
            else
                util::sendLine(member.first, fullMessage);
        }
    }
}
