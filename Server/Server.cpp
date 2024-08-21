//
// Created by roomi on 8/18/2024.
//

#include "Server.h"
#include "Command.h"
#include "UploadCommand.h"
#include "DownloadCommand.h"
#include "BroadcastCommand.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include <fstream>

Server::Server(int port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        exit(1);
    }

    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << "\n";
        WSACleanup();
        exit(1);
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }
}

Server::~Server() {
    stop();
}

void Server::start() {
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << "\n";
        closesocket(listenSocket);
        WSACleanup();
        exit(1);
    }

    std::cout << "Server started. Waiting for clients...\n";

    while (true) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << "\n";
            continue;
        }

        clients.push_back(clientSocket);
        // Handle the client in a separate thread
        std::thread clientThread(&Server::handleClient, this, clientSocket);
        // Detach the thread so it can run in the background
        clientThread.detach();
    }
}
void Server::handleClient(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        std::string command(buffer, bytesReceived);
        Command* cmd = nullptr;

        if (command.find("UPLOAD") == 0) {
            std::string filename = command.substr(7);
            cmd = new UploadCommand(clientSocket, filename, buffer, bytesReceived);
        }
        else if (command.find("DOWNLOAD") == 0) {
            std::string filename = command.substr(9);
            cmd = new DownloadCommand(clientSocket, filename, buffer);
        }
        else if (command.find("BROADCAST") == 0) {
            std::string message = command.substr(10); // Extract the message
            cmd = new BroadcastCommand(clients, message.c_str());
        }
        if(cmd != nullptr) {
            cmd->execute();
            delete cmd;
        }
        else {
            std::cerr << "Unknown command received from client.\n";
        }

    }
}


//void Server::handleClient(SOCKET clientSocket) {
//    char buffer[1024];
//    int bytesReceived;
//
//    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
//        std::string command(buffer, bytesReceived);
//
//        if (command.find("UPLOAD") == 0) {
//            std::string filename = command.substr(7);
//            upload(clientSocket, filename, buffer, bytesReceived);
//        }
//        else if (command.find("DOWNLOAD") == 0) {
//            std::string filename = command.substr(9);
//            download(clientSocket, filename, buffer);
//        }
//        else if (command.find("BROADCAST") == 0) {
//            std::string message = command.substr(10); // Extract the message
//            broadcast(message.c_str(), message.size());
//        }
//        else {
//            std::cerr << "Unknown command received from client.\n";
//        }
//
//    }
//}

//void Server::upload(SOCKET clientSocket, const std::string &filepath, char *buffer, int bytesReceived) {
//    std::ofstream outfile(filepath, std::ios::binary);
//    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
//        outfile.write(buffer, bytesReceived);
//    }
//    outfile.close();
//}
//
//void Server::download(SOCKET clientSocket, const std::string &filename, char *buffer) {
//    std::ifstream infile(filename, std::ios::binary);
//
//    if (infile.is_open()) {
//        while (infile.read(buffer, sizeof(buffer))) {
//            send(clientSocket, buffer, infile.gcount(), 0);
//        }
//        // Send any remaining bytes
//        send(clientSocket, buffer, infile.gcount(), 0);
//        infile.close();
//    }
//    closesocket(clientSocket); // Ensure connection is closed
//}
//
//void Server::broadcast(const char* message, int size) {
//    for (SOCKET clientSocket : clients) {
//        // Send the message to each client
//        send(clientSocket, message, size, 0);
//    }
//}

void Server::stop() {
    // Close all client sockets
    for (SOCKET clientSocket : clients) {
        closesocket(clientSocket);
    }
    closesocket(listenSocket);
    WSACleanup();
}