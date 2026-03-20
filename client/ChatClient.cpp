#include "ChatClient.h"
#include <iostream>
#include <cstring>

ChatClient::ChatClient()
    : sock(INVALID_SOCKET), initialized(false), running(true) {}

ChatClient::~ChatClient()
{
    stop();
    if (sock != INVALID_SOCKET)
        closesocket(sock);
    if (initialized)
        WSACleanup();
}

bool ChatClient::connectTo(const std::string &host, int port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed." << std::endl;
        return false;
    }
    initialized = true;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << " Socket creation error: " << WSAGetLastError() << std::endl;
        return false;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serverAddress.sin_addr) <= 0)
    {
        hostent *he = gethostbyname(host.c_str());
        if (!he)
        {
            std::cerr << " Host not found: " << host << std::endl;
            return false;
        }
        memcpy(&serverAddress.sin_addr, he->h_addr_list[0], he->h_length);
    }

    if (connect(sock, (sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << " Connection failed: " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

void ChatClient::login(const std::string &name)
{
    username = name;
    send(sock, username.c_str(), (int)username.size(), 0);
}

void ChatClient::startReceiving()
{
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);

    running = true;
    receiveThread = std::thread(&ChatClient::receiveLoop, this);
}

bool ChatClient::sendMessage(const std::string &message)
{
    if (send(sock, message.c_str(), (int)message.size(), 0) == SOCKET_ERROR)
    {
        std::cerr << " Send failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

void ChatClient::stop()
{
    running = false;
    if (receiveThread.joinable())
        receiveThread.join();
}

bool ChatClient::isRunning() const
{
    return running;
}

void ChatClient::receiveLoop()
{
    char buffer[1024];
    while (running)
    {
        int valread = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (valread > 0)
        {
            buffer[valread] = '\0';
            std::cout << "\r" << std::string(100, ' ') << "\r";
            std::cout << buffer << std::endl;
            std::cout << "[" << username << "]> " << std::flush;
        }
        else if (valread == 0)
        {
            std::cout << "\n  Server disconnected." << std::endl;
            running = false;
            break;
        }
        else
        {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK)
            {
                std::cout << "\n Connection error: " << error << std::endl;
                running = false;
                break;
            }
        }
    }
}
