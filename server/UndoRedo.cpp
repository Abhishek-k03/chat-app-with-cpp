#include "UndoRedo.h"

void UndoRedo::addMessage(const Message &message)
{
    std::lock_guard<std::mutex> lock(mtx);
    undoStack.push(message);
    while (!redoStack.empty())
        redoStack.pop();
}

bool UndoRedo::undo(Message &message)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (undoStack.empty())
        return false;
    message = undoStack.top();
    undoStack.pop();
    redoStack.push(message);
    return true;
}

bool UndoRedo::redo(Message &message)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (redoStack.empty())
        return false;
    message = redoStack.top();
    redoStack.pop();
    undoStack.push(message);
    return true;
}
