#include "History.h"

History::History(int maxSize)
    : head(nullptr), tail(nullptr), size(0), maxSize(maxSize) {}

History::~History()
{
    clear();
}

void History::addMessage(const Message &message)
{
    std::lock_guard<std::mutex> lock(mtx);

    Node *newNode = new Node(message);

    if (head == nullptr)
    {
        head = tail = newNode;
    }
    else
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }

    size++;

    if (size > maxSize)
    {
        Node *temp = head;
        head = head->next;
        if (head)
            head->prev = nullptr;
        delete temp;
        size--;
    }
}

void History::removeMessage(int messageId)
{
    std::lock_guard<std::mutex> lock(mtx);

    Node *current = head;
    while (current != nullptr)
    {
        if (current->message.id == messageId)
        {
            if (current->prev)
                current->prev->next = current->next;
            if (current->next)
                current->next->prev = current->prev;
            if (current == head)
                head = current->next;
            if (current == tail)
                tail = current->prev;

            delete current;
            size--;
            return;
        }
        current = current->next;
    }
}

std::list<Message> History::getMessages() const
{
    std::lock_guard<std::mutex> lock(mtx);
    std::list<Message> result;

    Node *current = head;
    while (current != nullptr)
    {
        result.push_back(current->message);
        current = current->next;
    }

    return result;
}

std::list<Message> History::searchMessages(const std::string &keyword) const
{
    std::lock_guard<std::mutex> lock(mtx);
    std::list<Message> result;

    Node *current = head;
    while (current != nullptr)
    {
        if (current->message.text.find(keyword) != std::string::npos)
        {
            result.push_back(current->message);
        }
        current = current->next;
    }

    return result;
}

void History::clear()
{
    std::lock_guard<std::mutex> lock(mtx);

    Node *current = head;
    while (current != nullptr)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }

    head = tail = nullptr;
    size = 0;
}
