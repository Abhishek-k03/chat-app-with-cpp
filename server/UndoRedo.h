#pragma once

#include <stack>
#include <mutex>
#include "Message.h"

class UndoRedo
{
private:
    std::stack<Message> undoStack;
    std::stack<Message> redoStack;
    std::mutex mtx;

public:
    void addMessage(const Message &message);
    bool undo(Message &message);
    bool redo(Message &message);
};
