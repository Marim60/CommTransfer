//
// Created by roomi on 8/21/2024.
//

#include <iostream>
#include <fstream>
#include <thread>
#include "Client.h"
Client::Client(const char *serverIp, int port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        exit(1);
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error at socket(): " << WSAGetLastError() << "\n";
        WSACleanup();
        exit(1);
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp);
    serverAddr.sin_port = htons(port);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed with error: " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        exit(1);
    }
    std::cout << "Connected to server!\n";
}

Client::~Client() {
    closesocket(clientSocket);
    WSACleanup();
}

void Client :: start()
{
    std::string command;
    std::string filePath;
    // Prompt user for command
    //do{
    std::cout << "Enter command (UPLOAD /path/to/file or DOWNLOAD filename or BROADCAST message): ";

    std::getline(std::cin, command);

    if (command.find("UPLOAD") == 0) {
        filePath = command.substr(7); // Extract file path
        handleUpload(filePath);
    } else if (command.find("DOWNLOAD") == 0) {
        filePath = command.substr(9); // Extract filename
        handleDownload(filePath);
    }
    else if(command.find("BROADCAST") == 0){
        handleBroadcast(command);
    }
    else {
        std::cerr << "Unknown command\n";
    }
    //}
    //while(command != "EXIT");
}

void Client::handleBroadcast(const std::string &message) {
    char buffer[1024];
    bool running = true;  // Flag to control the receive thread

    // Start a thread to receive messages from the server
    std::thread receiveThread([&]() {
        while (running) {
            int bytesReceived = recv(clientSocket, buffer, 1024, 0);
            if (bytesReceived > 0) {
                std::cout << "Server: " << std::string(buffer, 0, bytesReceived) << "\n";
            } else if (bytesReceived == 0 || !running) {
                std::cout << "Connection closed by server or client.\n";
                break;
            } else {
                std::cerr << "Receive failed with error: " << WSAGetLastError() << "\n";
                break;
            }
        }
    });

    // Send the initial broadcast message
    std::string messageToSend = "BROADCAST " + message;
    int sendResult = send(clientSocket, messageToSend.c_str(), messageToSend.size(), 0);
    if (sendResult == SOCKET_ERROR) {
        std::cerr << "Send failed with error: " << WSAGetLastError() << "\n";
    }

    // Allow the user to send more messages or exit
    std::string userInput;
    while (true) {
        std::getline(std::cin, userInput);
        if (userInput == "exit") {
            running = false;  // Signal the receive thread to stop
            closesocket(clientSocket);  // Close the socket to interrupt the recv call
            break;  // Exit the loop and end the function
        }

        messageToSend = "BROADCAST " + userInput;  // Prepend "BROADCAST " to each message
        sendResult = send(clientSocket, messageToSend.c_str(), messageToSend.size(), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "Send failed with error: " << WSAGetLastError() << "\n";
            break;
        }
    }

    receiveThread.join();  // Wait for the receive thread to finish before closing
}

void Client::handleDownload(const std::string& filename) {
    std::ofstream outfile(filename, std::ios::binary);
    std::string command = "DOWNLOAD " + filename;
    send(clientSocket, command.c_str(), command.size(), 0);

    char buffer[1024];
    int bytesReceived;

    size_t totalBytesReceived = 0; // Track total bytes received

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        outfile.write(buffer, bytesReceived);
        totalBytesReceived += bytesReceived;
    }

    if (bytesReceived == 0) {
        std::cout << "File downloaded successfully.\n";
    } else if (bytesReceived < 0) {
        std::cerr << "Error downloading file: " << WSAGetLastError() << "\n";
    }

    outfile.close();

    // Log the size of the downloaded file
    std::cout << "Total bytes received: " << totalBytesReceived << "\n";
}

void Client::handleUpload(const std::string& filePath) {
    std::ifstream infile(filePath, std::ios::binary);
    if (infile.is_open()) {
        std::string filename = filePath.substr(filePath.find_last_of("\\/") + 1);
        std::string command = "UPLOAD " + filename;
        send(clientSocket, command.c_str(), command.size(), 0);

        char buffer[1024];
        while (infile.read(buffer, sizeof(buffer))) {
            send(clientSocket, buffer, infile.gcount(), 0);
        }
        send(clientSocket, buffer, infile.gcount(), 0); // Send remaining bytes
        infile.close();
        std::cout << "File uploaded successfully.\n";
    } else {
        std::cerr << "File not found or cannot be opened.\n";
    }
}

