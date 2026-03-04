#include "CommandRouter.h"
#include "Commands.h"

CommandRouter::CommandRouter()
{
    commands["/join"] = std::make_unique<JoinCommand>();
    commands["/pm"] = std::make_unique<PrivateMessageCommand>();
    commands["/reply"] = std::make_unique<ReplyCommand>();
    commands["/undo"] = std::make_unique<UndoCommand>();
    commands["/redo"] = std::make_unique<RedoCommand>();
    commands["/search"] = std::make_unique<SearchCommand>();
    commands["/history"] = std::make_unique<HistoryCommand>();
    commands["/help"] = std::make_unique<HelpCommand>();
}

bool CommandRouter::dispatch(const std::string &input, ClientSession &session, ChatServices &services)
{
    size_t space = input.find(' ');
    std::string name = (space == std::string::npos) ? input : input.substr(0, space);
    std::string args = (space == std::string::npos) ? "" : input.substr(space + 1);

    auto it = commands.find(name);
    if (it == commands.end())
        return false;

    CommandContext context{session, args, services};
    it->second->execute(context);
    return true;
}
