#pragma once

#include <list>
#include <mutex>
#include <string>
#include "Message.h"

constexpr int MAX_MESSAGE_HISTORY = 1000;

class History
{
private:
    struct Node
    {
        Message message;
        Node *next;
        Node *prev;

        Node(const Message &message) : message(message), next(nullptr), prev(nullptr) {}
    };

    Node *head;
    Node *tail;
    int size;
    int maxSize;
    mutable std::mutex mtx;

public:
    explicit History(int maxSize = MAX_MESSAGE_HISTORY);
    ~History();

    void addMessage(const Message &message);
    void removeMessage(int messageId);
    std::list<Message> getMessages() const;
    std::list<Message> searchMessages(const std::string &keyword) const;
    void clear();
};
