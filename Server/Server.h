//
// Created by roomi on 8/18/2024.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include <winsock.h>
#include <vector>
#include <string>

class Server {
public:
    Server(int port);
    ~Server();
    void start();
    void stop();

private:
    SOCKET listenSocket;
    std::vector<SOCKET> clients;
    void handleClient(SOCKET clientSocket);

//    void download(SOCKET clientSocket, const std::string& filename, char *buffer);
//    void upload(SOCKET clientSocket, const std::string& filepath, char *buffer, int bytesReceived);
//    void broadcast(const char* message, int messageSize);
};


#endif //SERVER_SERVER_H
