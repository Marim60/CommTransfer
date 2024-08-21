//
// Created by roomi on 8/21/2024.
//

#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H


class Command {
    public:
    virtual void execute() = 0;
    virtual ~Command() = default;
};


#endif //SERVER_COMMAND_H
