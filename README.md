# Server-Client Application in C++

## Overview

This project implements a basic server-client application in C++ using socket programming. The server allows multiple clients to connect and provides the following functionalities:
- **File Upload**: Clients can upload files to the server.
- **File Download**: Clients can download files from the server.
- **Message Broadcasting**: Clients can broadcast messages to all other connected clients.

## Features

- **Multi-client support**: The server can handle multiple clients simultaneously, each in its own thread.
- **File Transfer**: Efficient file transfer operations for both upload and download.
- **Message Broadcasting**: Real-time messaging between all connected clients.
- **Extensible Design**: The project is designed following SOLID principles, making it easy to extend with additional features.

## Getting Started

### Prerequisites

- Windows OS (due to usage of `winsock`)
- C++ compiler (e.g., MinGW, Visual Studio)
- Basic knowledge of socket programming in C++

### Building the Project

1. Clone the repository:
   ```
   git clone https://github.com/yourusername/your-repo-name.git
   ```
2. Navigate to the project directory:
   ```
   cd your-repo-name
   ```
3. Compile the server and client:
   ```
     g++ -o server Server.cpp -lws2_32
     g++ -o client Client.cpp -lws2_32
   ```
4. Running the Application:
   Start the server:
   ```
    ./server
   ```
   Start the client(s):
   ```
    ./client
   ```
### Usage
- Upload a File: UPLOAD /path/to/your/file
- Download a File: DOWNLOAD filename
- Broadcast a Message: BROADCAST your_message_here
- Exit the Brodcast: exit
