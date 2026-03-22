#include "ChatClient.h"
#include <iostream>
#include <string>
#include <windows.h>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

static void printCommands()
{
    cout << "==========================================" << endl;
    cout << "          CHAT APPLICATION COMMANDS       " << endl;
    cout << "==========================================" << endl;
    cout << "/join <room>           - Join or create a chat room" << endl;
    cout << "/pm <user> <message>   - Send private message to a user" << endl;
    cout << "/reply <user> <msg>    - Reply to a user in the current room" << endl;
    cout << "/undo                  - Undo your last message" << endl;
    cout << "/redo                  - Redo your last undone message" << endl;
    cout << "/history               - Show message history for current room" << endl;
    cout << "/search <keyword>      - Search for messages containing keyword" << endl;
    cout << "/quit                  - Exit the chat application" << endl;
    cout << "/help                  - Show this help message" << endl;
    cout << "==========================================" << endl;
}

int main()
{
    string username;
    cout << "Enter your username: ";
    getline(cin, username);

    string host;
    int port;
    cout << "Enter server hostname/IP (e.g. 127.0.0.1): ";
    getline(cin, host);
    cout << "Enter server port (e.g. 8080): ";
    cin >> port;
    cin.ignore();

    ChatClient client;
    if (!client.connectTo(host, port))
        return -1;

    client.login(username);
    printCommands();
    client.startReceiving();

    string message;
    while (client.isRunning())
    {
        cout << "[" << username << "]> " << std::flush;
        getline(cin, message);

        if (message == "/quit")
        {
            client.stop();
            break;
        }

        if (!client.sendMessage(message))
        {
            client.stop();
            break;
        }

        Sleep(100);
    }

    cout << username + " Disconnected from server." << endl;
    return 0;
}
