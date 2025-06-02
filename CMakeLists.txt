cmake_minimum_required(VERSION 3.10)
project(MultithreadedProxyServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/proxy.cpp
    src/auth.cpp
    src/logger.cpp
)
add_executable(MultithreadedProxyServer ${SOURCES})
target_link_libraries(MultithreadedProxyServer PRIVATE ws2_32)
