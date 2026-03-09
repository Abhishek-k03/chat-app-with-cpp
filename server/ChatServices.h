#pragma once

#include <atomic>

class ClientRegistry;
class History;
class UndoRedo;
class MessageQueue;

struct ChatServices
{
    ClientRegistry &registry;
    History &history;
    UndoRedo &undoRedo;
    MessageQueue &queue;
    std::atomic<int> &counter;
};
