#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <atomic>
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

class ChatClient
{
private:
    SOCKET sock;
    bool initialized;
    std::string username;
    std::atomic<bool> running;
    std::thread receiveThread;

    void receiveLoop();

public:
    ChatClient();
    ~ChatClient();

    bool connectTo(const std::string &host, int port);
    void login(const std::string &name);
    void startReceiving();
    bool sendMessage(const std::string &message);
    void stop();

    bool isRunning() const;
};
