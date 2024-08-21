// BroadcastCommand.h
#ifndef SERVER_BROADCASTCOMMAND_H
#define SERVER_BROADCASTCOMMAND_H

#include "Command.h"
#include <winsock.h>
#include <vector>
#include <string>

class BroadcastCommand : public Command {
public:
    BroadcastCommand(const std::vector<SOCKET>& clients, const std::string& message)
            : clients(clients), message(message) {}

    void execute() override {
        for (SOCKET clientSocket : clients) {
            send(clientSocket, message.c_str(), message.size(), 0);
        }
    }

private:
    std::vector<SOCKET> clients;
    std::string message;
};

#endif //SERVER_BROADCASTCOMMAND_H
