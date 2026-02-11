#include "ClientRegistry.h"
#include "Utils.h"

void ClientRegistry::add(SOCKET socket, const std::string &username, const std::string &room)
{
    std::lock_guard<std::mutex> lock(mtx);
    clients[socket] = username;
    rooms[room].insert(socket);
}

void ClientRegistry::removeClient(SOCKET socket)
{
    std::lock_guard<std::mutex> lock(mtx);
    clients.erase(socket);
}

void ClientRegistry::joinRoom(SOCKET socket, const std::string &room)
{
    std::lock_guard<std::mutex> lock(mtx);
    rooms[room].insert(socket);
}

void ClientRegistry::leaveRoom(SOCKET socket, const std::string &room)
{
    std::lock_guard<std::mutex> lock(mtx);
    rooms[room].erase(socket);
}

SOCKET ClientRegistry::findByUsername(const std::string &username) const
{
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto &entry : clients)
    {
        if (entry.second == username)
            return entry.first;
    }
    return INVALID_SOCKET;
}

std::string ClientRegistry::roomOfUser(const std::string &username) const
{
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto &room : rooms)
    {
        for (SOCKET socket : room.second)
        {
            auto it = clients.find(socket);
            if (it != clients.end() && it->second == username)
                return room.first;
        }
    }
    return "";
}

std::vector<std::pair<SOCKET, std::string>> ClientRegistry::snapshotRoom(const std::string &room) const
{
    std::lock_guard<std::mutex> lock(mtx);
    std::vector<std::pair<SOCKET, std::string>> members;

    auto it = rooms.find(room);
    if (it != rooms.end())
    {
        for (SOCKET socket : it->second)
        {
            auto client = clients.find(socket);
            if (client != clients.end())
                members.emplace_back(socket, client->second);
        }
    }
    return members;
}

void ClientRegistry::broadcast(const std::string &room, const std::string &message, SOCKET except) const
{
    std::vector<SOCKET> targets;
    {
        std::lock_guard<std::mutex> lock(mtx);
        auto it = rooms.find(room);
        if (it != rooms.end())
        {
            for (SOCKET socket : it->second)
            {
                if (socket != except)
                    targets.push_back(socket);
            }
        }
    }

    for (SOCKET socket : targets)
        util::sendLine(socket, message);
}
