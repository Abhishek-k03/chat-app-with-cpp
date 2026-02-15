#pragma once

#include <thread>
#include <atomic>

class ClientRegistry;
class MessageQueue;

class Broadcaster
{
private:
    ClientRegistry &registry;
    MessageQueue &queue;
    std::thread worker;
    std::atomic<bool> running{false};

    void run();

public:
    Broadcaster(ClientRegistry &registry, MessageQueue &queue);
    ~Broadcaster();

    void start();
    void stop();
};
