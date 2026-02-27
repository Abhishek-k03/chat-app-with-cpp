#pragma once

#include "Command.h"

class JoinCommand : public ICommand
{
public:
    void execute(CommandContext &context) override;
};

class PrivateMessageCommand : public ICommand
{
public:
    void execute(CommandContext &context) override;
};

class ReplyCommand : public ICommand
{
public:
    void execute(CommandContext &context) override;
};

class UndoCommand : public ICommand
{
public:
    void execute(CommandContext &context) override;
};

class RedoCommand : public ICommand
{
public:
    void execute(CommandContext &context) override;
};

class SearchCommand : public ICommand
{
public:
    void execute(CommandContext &context) override;
};

class HistoryCommand : public ICommand
{
public:
    void execute(CommandContext &context) override;
};

class HelpCommand : public ICommand
{
public:
    void execute(CommandContext &context) override;
};
