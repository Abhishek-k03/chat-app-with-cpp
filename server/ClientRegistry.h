#pragma once

#include <map>
#include <set>
#include <mutex>
#include <string>
#include <vector>
#include <utility>
#include "Platform.h"

class ClientRegistry
{
private:
    std::map<SOCKET, std::string> clients;
    std::map<std::string, std::set<SOCKET>> rooms;
    mutable std::mutex mtx;

public:
    void add(SOCKET socket, const std::string &username, const std::string &room);
    void removeClient(SOCKET socket);
    void joinRoom(SOCKET socket, const std::string &room);
    void leaveRoom(SOCKET socket, const std::string &room);

    SOCKET findByUsername(const std::string &username) const;
    std::string roomOfUser(const std::string &username) const;
    std::vector<std::pair<SOCKET, std::string>> snapshotRoom(const std::string &room) const;

    void broadcast(const std::string &room, const std::string &message, SOCKET except = INVALID_SOCKET) const;
};
