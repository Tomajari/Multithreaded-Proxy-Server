#ifndef PROXY_H
#define PROXY_H

#include <string>


class ProxyServer {
public:
    ProxyServer(int port, const std::string& backendHost, int backendPort);
    void start();

private:
    int serverPort;
    std::string backendHost;
    int backendPort;
    int serverSocket;

    // Updated function declarations for POSIX socket type
    void handleClient(int clientSocket);
    void forwardToBackend(const std::string& request, int clientSocket);
};

#endif // PROXY_H
