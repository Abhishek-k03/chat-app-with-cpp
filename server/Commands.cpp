#include "Commands.h"
#include "ClientRegistry.h"
#include "History.h"
#include "UndoRedo.h"
#include "MessageQueue.h"
#include "Message.h"
#include "Utils.h"

void JoinCommand::execute(CommandContext &context)
{
    ClientSession &session = context.session;
    ChatServices &services = context.services;

    std::string room = util::trim(context.args);
    if (room.empty())
    {
        util::sendLine(session.socket, "[" + util::currentTime() + "] Usage: /join <room>\n");
        return;
    }

    std::string oldRoom = session.currentRoom;

    services.registry.leaveRoom(session.socket, oldRoom);
    services.registry.broadcast(oldRoom, "[" + util::currentTime() + "] " + session.username + " left the room\n");

    session.currentRoom = room;
    services.registry.joinRoom(session.socket, room);
    services.registry.broadcast(room, "[" + util::currentTime() + "] " + session.username + " joined the room\n", session.socket);

    util::sendLine(session.socket, "[" + util::currentTime() + "] You joined room: " + room + "\n");
}

void PrivateMessageCommand::execute(CommandContext &context)
{
    ClientSession &session = context.session;

    std::string target, text;
    if (!util::splitFirst(context.args, target, text))
    {
        util::sendLine(session.socket, "[" + util::currentTime() + "] Usage: /pm <user> <message>\n");
        return;
    }

    SOCKET targetSocket = context.services.registry.findByUsername(target);
    if (targetSocket != INVALID_SOCKET)
    {
        util::sendLine(targetSocket, "[" + util::currentTime() + "][PM from " + session.username + "]: " + text + "\n");
        util::sendLine(session.socket, "[" + util::currentTime() + "][PM to " + target + "]: " + text + "\n");
    }
    else
    {
        util::sendLine(session.socket, "[" + util::currentTime() + "] User not found.\n");
    }
}

void ReplyCommand::execute(CommandContext &context)
{
    ClientSession &session = context.session;
    ChatServices &services = context.services;

    std::string target, text;
    if (!util::splitFirst(context.args, target, text))
    {
        util::sendLine(session.socket, "[" + util::currentTime() + "] Usage: /reply <user> <message>\n");
        return;
    }

    SOCKET targetSocket = services.registry.findByUsername(target);
    if (targetSocket != INVALID_SOCKET)
    {
        Message message(services.counter++, session.username, "-> " + target + ": " + text);
        services.history.addMessage(message);
        services.undoRedo.addMessage(message);
        services.queue.push(message);
    }
    else
    {
        util::sendLine(session.socket, "[" + util::currentTime() + "] User '" + target + "' not found.\n");
    }
}

void UndoCommand::execute(CommandContext &context)
{
    ChatServices &services = context.services;

    Message last;
    if (services.undoRedo.undo(last))
    {
        services.history.removeMessage(last.id);
        util::sendLine(context.session.socket, "[" + util::currentTime() + "] Last message undone.\n");
    }
    else
    {
        util::sendLine(context.session.socket, "[" + util::currentTime() + "] No message to undo.\n");
    }
}

void RedoCommand::execute(CommandContext &context)
{
    ChatServices &services = context.services;

    Message message;
    if (services.undoRedo.redo(message))
    {
        services.history.addMessage(message);
        services.queue.push(message);
        util::sendLine(context.session.socket, "[" + util::currentTime() + "] Message redone.\n");
    }
    else
    {
        util::sendLine(context.session.socket, "[" + util::currentTime() + "] Nothing to redo.\n");
    }
}

void SearchCommand::execute(CommandContext &context)
{
    ClientSession &session = context.session;

    std::string keyword = context.args;
    if (keyword.empty())
    {
        util::sendLine(session.socket, "[" + util::currentTime() + "] Usage: /search <keyword>\n");
        return;
    }

    auto results = context.services.history.searchMessages(keyword);
    if (results.empty())
    {
        util::sendLine(session.socket, "[" + util::currentTime() + "] No messages found containing: '" + keyword + "'\n");
    }
    else
    {
        std::string reply = "[" + util::currentTime() + "] Found " + std::to_string(results.size()) +
                            " message(s) containing '" + keyword + "':\n";
        for (const auto &message : results)
            reply += message.toString() + "\n";
        util::sendLine(session.socket, reply);
    }
}

void HistoryCommand::execute(CommandContext &context)
{
    auto messages = context.services.history.getMessages();

    if (messages.empty())
    {
        util::sendLine(context.session.socket, "[" + util::currentTime() + "] No message history available.\n");
        return;
    }

    std::string reply = "[" + util::currentTime() + "] Message history:\n";
    for (const auto &message : messages)
        reply += message.toString() + "\n";
    util::sendLine(context.session.socket, reply);
}

void HelpCommand::execute(CommandContext &context)
{
    std::string help =
        "[" + util::currentTime() + "] Available commands:\n"
        "/join <room>           - Join or create a chat room\n"
        "/pm <user> <message>   - Send private message to a user\n"
        "/reply <user> <msg>    - Reply publicly to a specific user in the room\n"
        "/undo                  - Undo your last message\n"
        "/redo                  - Redo your last undone message\n"
        "/history               - Show message history for current room\n"
        "/search <keyword>      - Search for messages containing keyword\n"
        "/quit                  - Exit the chat application\n"
        "/help                  - Show this help message\n";
    util::sendLine(context.session.socket, help);
}
