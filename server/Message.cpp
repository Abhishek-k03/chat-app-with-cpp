#include "Message.h"
#include "Utils.h"

Message::Message(int id, std::string sender, std::string text)
    : id(id), sender(sender), text(text)
{
    timestamp = time(nullptr);
}

std::string Message::toString() const
{
    return util::formatMessage(sender, text);
}
