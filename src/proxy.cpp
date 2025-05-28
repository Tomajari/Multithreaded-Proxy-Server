#include "../include/proxy.h"
#include "../include/auth.h"
#include "../include/logger.h"
#include <iostream>
#include <winsock2.h>  // Windows-specific socket functions
#include <ws2tcpip.h>  // For inet_pton and other IP utilities
#include <cstring>
#include <thread>

// Link with Winsock library
#pragma comment(lib, "ws2_32.lib")

ProxyServer::ProxyServer(int port, const std::string& backendHost, int backendPort)
    : serverPort(port), backendHost(backendHost), backendPort(backendPort) {}

void ProxyServer::start() {
    sockaddr_in serverAddr{};
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "❌ Error: Could not create socket." << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "❌ Error: Bind failed. Port might be in use." << std::endl;
        return;
    }

    listen(serverSocket, 10);
    std::cout << "[RebelGate] Listening on port " << serverPort << "..." << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket != INVALID_SOCKET) {
            std::thread(&ProxyServer::handleClient, this, clientSocket).detach();
        }
    }
}

void ProxyServer::handleClient(SOCKET clientSocket) {
    char buffer[4096] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        closesocket(clientSocket);
        return;
    }

    std::string request(buffer);
    std::string clientIP = "127.0.0.1"; // In production, extract from sockaddr

    if (!Auth::isAuthorized(request)) {
        std::string response = "HTTP/1.1 403 Forbidden\r\nContent-Length: 0\r\n\r\n";
        send(clientSocket, response.c_str(), response.size(), 0);
        Logger::logRequest(clientIP, "UNKNOWN", "N/A", 403);
        closesocket(clientSocket);
        return;
    }

    if (Auth::isRateLimited(clientIP)) {
        std::string response = "HTTP/1.1 429 Too Many Requests\r\nContent-Length: 0\r\n\r\n";
        send(clientSocket, response.c_str(), response.size(), 0);
        Logger::logRequest(clientIP, "UNKNOWN", "N/A", 429);
        closesocket(clientSocket);
        return;
    }

    forwardToBackend(request, clientSocket);
    closesocket(clientSocket);
}

void ProxyServer::forwardToBackend(const std::string& request, SOCKET clientSocket) {
    SOCKET backendSocket = socket(AF_INET, SOCK_STREAM, 0);
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
        std::cerr << "⚠️ Could not connect to backend server.\n";
    }

    closesocket(backendSocket);
}
