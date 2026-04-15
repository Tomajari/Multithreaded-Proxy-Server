#include "../include/logger.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#if __cplusplus >= 201703L
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <sys/stat.h>
#endif
#include <iostream>   

void Logger::logRequest(const std::string& ip, const std::string& method, const std::string& path, int status) {

    // Ensure the logs directory exists
#if __cplusplus >= 201703L
    fs::create_directory("logs");
#else
    struct stat st = {0};
    if (stat("logs", &st) == -1) {
        mkdir("logs", 0700);
    }
#endif

    std::ofstream logFile("logs/rebelgate.log", std::ios::app); // Open in append mode

    if (!logFile.is_open()) {
        std::cerr << " Logger: Failed to open logs/rebelgate.log" << std::endl;
        return;
    }

    // Get current time in readable format
    auto now = std::time(nullptr);
    std::stringstream timeStream;
    timeStream << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");

    // Format and write log entry
    logFile << "[" << timeStream.str() << "] "
            << "IP: " << ip << " | "
            << "Method: " << method << " | "
            << "Path: " << path << " | "
            << "Status: " << status << std::endl;

    logFile.close();
}
