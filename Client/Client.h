//
// Created by roomi on 8/21/2024.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H
#include <winsock.h>
#include <string>

class Client {
public:
    Client(const char* serverIp, int port);
    ~Client();
    void start();

private:
    void handleDownload(const std::string& filename);
    void handleUpload(const std::string& filepath);
    void handleBroadcast(const std::string& message);
    SOCKET clientSocket;

};


#endif //CLIENT_CLIENT_H
