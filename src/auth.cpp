#include "../include/auth.h"
#include <unordered_map>
#include <chrono>

static const std::string VALID_TOKEN = "Authorization: Bearer secret-token";

// Simple map to store last request time per IP
static std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastRequestTime;

bool Auth::isAuthorized(const std::string& request) {
    return request.find(VALID_TOKEN) != std::string::npos;
}

bool Auth::isRateLimited(const std::string& ip) {
    using namespace std::chrono;

    auto now = steady_clock::now();

    if (lastRequestTime.find(ip) != lastRequestTime.end()) {
        auto duration = duration_cast<seconds>(now - lastRequestTime[ip]);
        if (duration.count() < 2) {
            return true; // Limit to 1 request every 2 seconds per IP
        }
    }

    lastRequestTime[ip] = now;
    return false;
}
