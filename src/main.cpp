#include "../include/proxy.h"
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    int port = 8080;                // Port that RebelGate will listen on
    std::string backendHost = "127.0.0.1"; // Backend server 
    int backendPort = 9000;         // Port that backend server is running on

    std::cout << "Starting RebelGate on port " << port << "..." << std::endl;

    ProxyServer server(port, backendHost, backendPort);
    server.start(); // listens and starts requests

    WSACleanup();
    return 0;
}
