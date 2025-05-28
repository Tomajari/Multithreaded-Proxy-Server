#ifndef PROXY_H
#define PROXY_H

#include <string>
#include <winsock2.h>

class ProxyServer {
public:
    ProxyServer(int port, const std::string& backendHost, int backendPort);
    void start();

private:
    int serverPort;
    std::string backendHost;
    int backendPort;
    SOCKET serverSocket;

    // Updated function declarations for SOCKET type
    void handleClient(SOCKET clientSocket);
    void forwardToBackend(const std::string& request, SOCKET clientSocket);
};

#endif // PROXY_H
