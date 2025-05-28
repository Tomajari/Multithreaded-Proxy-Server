#ifndef AUTH_H
#define AUTH_H

#include <string>

namespace Auth {
    // Check if a request contains the valid Authorization token
    bool isAuthorized(const std::string& request);

    // Limit repeated requests from the same IP (rudimentary rate limiting)
    bool isRateLimited(const std::string& ip);
}

#endif // AUTH_H
