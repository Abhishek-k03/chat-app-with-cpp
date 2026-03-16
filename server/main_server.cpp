#include "ChatServer.h"

int main()
{
    ChatServer server(DEFAULT_PORT);
    if (!server.start())
        return 1;

    server.run();
    return 0;
}
