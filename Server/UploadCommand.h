// UploadCommand.h
#ifndef SERVER_UPLOADCOMMAND_H
#define SERVER_UPLOADCOMMAND_H

#include "Command.h"
#include <winsock.h>
#include <fstream>
#include <string>

class UploadCommand : public Command {
public:
    UploadCommand(SOCKET clientSocket, const std::string& filepath, char* buffer, int bytesReceived)
            : clientSocket(clientSocket), filepath(filepath), buffer(buffer), bytesReceived(bytesReceived) {}

    void execute() override {
        std::ofstream outfile(filepath, std::ios::binary);
        while ((bytesReceived = recv(clientSocket, buffer, 1024, 0)) > 0) {
            outfile.write(buffer, bytesReceived);
        }
        outfile.close();
    }

private:
    SOCKET clientSocket;
    std::string filepath;
    char* buffer;
    int bytesReceived;
};

#endif //SERVER_UPLOADCOMMAND_H
