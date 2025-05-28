#ifndef LOGGER_H
#define LOGGER_H

#include <string>

namespace Logger {
    // Log the details of an HTTP request
    void logRequest(const std::string& ip, const std::string& method, const std::string& path, int status);
}

#endif // LOGGER_H
