# Multithreaded Proxy Server
RebelGate, is a multithreaded reverse proxy server that can handle multiple client requests concurrently using C++ threads. It includes features like request forwarding, token-based authorization, rate limiting, and logging.  
Skills Demonstrated: Multithreading, Network Programming, Sockets, HTTP Proxying, Authorization, Logging.

---

## Overview

This project implements a reverse proxy server in C++. The server listens for HTTP requests on a specified port, validates them using a bearer token, applies per-IP rate limiting, and forwards authorized requests to a backend server. Unauthorized or too-frequent requests are rejected with appropriate HTTP status codes. All activity is logged for debugging and monitoring purposes.

**Note**: This version is designed for Windows using Winsock. It can be adapted for POSIX-based systems with minor changes.

---

## Project Structure

- **CMakeLists.txt**: Configuration file for building the project with CMake.
- **include/proxy.h**: Header file declaring the `ProxyServer` class.
- **include/auth.h**: Header file for token validation and rate limiting functions.
- **include/logger.h**: Header file for the request logger.
- **src/main.cpp**: Main entry point of the application.
- **src/proxy.cpp**: Core logic for accepting connections and forwarding requests.
- **src/auth.cpp**: Checks authorization token and applies rate-limiting logic.
- **src/logger.cpp**: Logs request metadata to `rebelgate.log`.
- **README.md**: Documentation file for the project.

---

## Key Features

- **Multithreading**: Handles multiple client connections concurrently using `std::thread`.
- **Authorization**: Only requests with a valid bearer token are forwarded.
- **Rate Limiting**: Limits requests per IP to avoid abuse (1 request every 2 seconds).
- **Proxy Forwarding**: Forwards valid requests to a backend server.
- **Logging**: All requests (IP, method, path, status) are logged to a local file.
- **Modular Design**: Code is cleanly organized into headers and implementation files.

---

## Building the Project

To build the project, follow these steps:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Tomajari/Multithreaded-Proxy-Server.git
   cd Multithreaded-Proxy-Server
   ```

2. **Create a Build Directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Generate Build Files with CMake**:
   ```bash
   cmake ..
   ```

4. **Compile the Project**:
   ```bash
   cmake --build .
   ```

---

## Running the Proxy Server

After building the project, you can run the proxy server with the following command:

```bash
./Multithreaded-Proxy-Server
```

The server will start and listen for incoming connections on the specified port (default is 8080). You can modify the port in `src/main.cpp`.

---

## Testing the Proxy Server

To test the server, you can use tools like `curl` or a web browser:

- **Valid Requests**: Send a request with a valid bearer token to the proxy server. The request will be forwarded to the backend server.
- **Unauthorized Requests**: Requests without a valid token will receive a `403 Forbidden` response.
- **Rate-Limited Requests**: Excessive requests from the same IP will receive a `429 Too Many Requests` response.

Example using `curl`:
```bash
curl -H "Authorization: Bearer secret-token" http://localhost:8080
```

---

## Customization

- **Port Number**: Change the port number in `src/main.cpp` to any available port.
- **Backend Server**: Update the backend server's IP and port in `src/main.cpp`.
- **Rate Limiting**: Modify the rate-limiting logic in `src/auth.cpp` to adjust the request limits.
- **Logging**: Customize the log format or file location in `src/logger.cpp`.

---

## Troubleshooting

- **Port Already in Use**: If you encounter a `bind failed: Address already in use` error, ensure no other process is using the specified port, or change the port number.
- **Backend Server Unreachable**: If the proxy server cannot connect to the backend server, verify the backend server's IP and port.
- **File Permissions**: Ensure the application has write permissions for the `logs/` directory.
---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
