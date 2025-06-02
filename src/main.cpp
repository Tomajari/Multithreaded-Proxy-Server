#include "../include/proxy.h"
#include <iostream>

int main() {
    int port = 8080;                // Port that RebelGate will listen on
    std::string backendHost = "127.0.0.1"; // Backend server 
    int backendPort = 9000;         // Backend server port

    std::cout << "Starting RebelGate on port " << port << "..." << std::endl;

    ProxyServer server(port, backendHost, backendPort);
    server.start(); // listens and starts requests

    return 0;
}
