#include "ChatServer.h"
#include "ClientHandler.h"
#include "Utils.h"
#include <iostream>
#include <thread>

ChatServer::ChatServer(int port)
    : port(port),
      listenSocket(INVALID_SOCKET),
      initialized(false),
      history(MAX_MESSAGE_HISTORY),
      counter(0),
      services{registry, history, undoRedo, queue, counter},
      broadcaster(registry, queue)
{
}

ChatServer::~ChatServer()
{
    broadcaster.stop();
    if (listenSocket != INVALID_SOCKET)
        closesocket(listenSocket);
    if (initialized)
        WSACleanup();
}

bool ChatServer::start()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return false;
    }
    initialized = true;

    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    int opt = 1;
    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(listenSocket, (sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

void ChatServer::run()
{
    std::cout << "[" << util::currentTime() << "] Chat server started on port " << port << std::endl;

    broadcaster.start();

    while (true)
    {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "[" << util::currentTime() << "] Accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        std::cout << "[" << util::currentTime() << "] New connection accepted.\n";
        std::thread(&ChatServer::handleConnection, this, clientSocket).detach();
    }
}

void ChatServer::handleConnection(SOCKET clientSocket)
{
    ClientHandler handler(clientSocket, services, router);
    handler.run();
}
