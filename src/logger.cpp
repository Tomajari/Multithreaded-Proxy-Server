#include "../include/logger.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem> 
#include <iostream>   

void Logger::logRequest(const std::string& ip, const std::string& method, const std::string& path, int status) {
    // Ensure the logs directory exists
    std::filesystem::create_directory("logs");

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
