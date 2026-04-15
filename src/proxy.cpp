#include "../include/proxy.h"
#include "../include/auth.h"
#include "../include/logger.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>



ProxyServer::ProxyServer(int port, const std::string& backendHost, int backendPort)
    : serverPort(port), backendHost(backendHost), backendPort(backendPort) {}

void ProxyServer::start() {
    sockaddr_in serverAddr{};


    std::cout << "[DEBUG] Creating socket..." << std::endl;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "[DEBUG] ERROR: Could not create socket." << std::endl;
        perror("socket");
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;



    std::cout << "[DEBUG] Binding socket to port " << serverPort << "..." << std::endl;
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "[DEBUG] ERROR: Bind failed. Port might be in use." << std::endl;
        perror("bind");
        return;
    }


    std::cout << "[DEBUG] Listening..." << std::endl;
    if (listen(serverSocket, 10) < 0) {
        std::cerr << "[DEBUG] ERROR: Listen failed." << std::endl;
        perror("listen");
        return;
    }
    std::cout << "[RebelGate] Listening on port " << serverPort << "..." << std::endl;

    while (true) {
        std::cout << "[DEBUG] Waiting for client connection..." << std::endl;
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket >= 0) {
            std::cout << "[DEBUG] Accepted client connection: fd=" << clientSocket << std::endl;
            std::thread(&ProxyServer::handleClient, this, clientSocket).detach();
        } else {
            perror("accept");
        }
    }
}

void ProxyServer::handleClient(int clientSocket) {
    char buffer[4096] = {0};

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        close(clientSocket);
        return;
    }

    std::string request(buffer);
    std::string clientIP = "127.0.0.1"; //extract from sockaddr

    if (!Auth::isAuthorized(request)) {
        std::string response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
        send(clientSocket, response.c_str(), response.size(), 0);
        Logger::logRequest(clientIP, "UNKNOWN", "N/A", 403);
        close(clientSocket);
        return;
    }

    if (Auth::isRateLimited(clientIP)) {
        std::string response = "HTTP/1.1 429 Too Many Requests\r\nContent-Length: 0\r\n\r\n";
        send(clientSocket, response.c_str(), response.size(), 0);
        Logger::logRequest(clientIP, "UNKNOWN", "N/A", 429);
        close(clientSocket);
        return;
    }

    forwardToBackend(request, clientSocket);
    close(clientSocket);
}

void ProxyServer::forwardToBackend(const std::string& request, int clientSocket) {
    int backendSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in backendAddr{};

    backendAddr.sin_family = AF_INET;
    backendAddr.sin_port = htons(backendPort);
    inet_pton(AF_INET, backendHost.c_str(), &backendAddr.sin_addr);

    if (connect(backendSocket, (sockaddr*)&backendAddr, sizeof(backendAddr)) == 0) {
        send(backendSocket, request.c_str(), request.size(), 0);

        char buffer[4096];
        int bytesRead = recv(backendSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            send(clientSocket, buffer, bytesRead, 0);
        }
    } else {
        std::cerr << "ERROR: Could not connect to backend server.\n";
    }

    close(backendSocket);
}
