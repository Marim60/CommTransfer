// DownloadCommand.h
#ifndef SERVER_DOWNLOADCOMMAND_H
#define SERVER_DOWNLOADCOMMAND_H

#include "Command.h"
#include <winsock.h>
#include <fstream>
#include <string>

class DownloadCommand : public Command {
public:
    DownloadCommand(SOCKET clientSocket, const std::string& filename, char* buffer)
            : clientSocket(clientSocket), filename(filename), buffer(buffer) {}

    void execute() override {
        std::ifstream infile(filename, std::ios::binary);
        if (infile.is_open()) {
            while (infile.read(buffer, 1024)) {
                send(clientSocket, buffer, infile.gcount(), 0);
            }
            // Send any remaining bytes
            send(clientSocket, buffer, infile.gcount(), 0);
            infile.close();
        }
        closesocket(clientSocket);
    }

private:
    SOCKET clientSocket;
    std::string filename;
    char* buffer;
};

#endif //SERVER_DOWNLOADCOMMAND_H
