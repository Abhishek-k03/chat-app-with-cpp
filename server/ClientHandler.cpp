#include "ClientHandler.h"
#include "CommandRouter.h"
#include "ClientRegistry.h"
#include "MessageQueue.h"
#include "History.h"
#include "UndoRedo.h"
#include "Message.h"
#include "Utils.h"

ClientHandler::ClientHandler(SOCKET socket, ChatServices &services, CommandRouter &router)
    : socket(socket), services(services), router(router) {}

bool ClientHandler::readUsername(std::string &username)
{
    char buffer[1024];
    int valread = recv(socket, buffer, sizeof(buffer) - 1, 0);
    if (valread <= 0)
        return false;

    buffer[valread] = '\0';
    username = std::string(buffer);
    return true;
}

void ClientHandler::postMessage(const ClientSession &session, const std::string &text)
{
    Message message(services.counter++, session.username, text);
    services.history.addMessage(message);
    services.undoRedo.addMessage(message);
    services.queue.push(message);
}

void ClientHandler::run()
{
    ClientSession session{socket, "", "chatroom"};

    if (!readUsername(session.username))
    {
        closesocket(socket);
        return;
    }

    services.registry.add(socket, session.username, session.currentRoom);

    std::string welcome = "[" + util::currentTime() + "] Connected as '" + session.username +
                          "' to chat server. You are in room: " + session.currentRoom + "\n";
    util::sendLine(socket, welcome);

    services.registry.broadcast(session.currentRoom,
                                "[" + util::currentTime() + "] " + session.username + " joined the room\n",
                                socket);

    char buffer[1024];
    while (true)
    {
        int valread = recv(socket, buffer, sizeof(buffer) - 1, 0);
        if (valread <= 0)
        {
            services.registry.leaveRoom(socket, session.currentRoom);
            services.registry.broadcast(session.currentRoom,
                                        "[" + util::currentTime() + "] " + session.username + " left the room\n");
            services.registry.removeClient(socket);
            closesocket(socket);
            break;
        }

        buffer[valread] = '\0';
        std::string input(buffer);

        if (!router.dispatch(input, session, services))
            postMessage(session, input);
    }
}
