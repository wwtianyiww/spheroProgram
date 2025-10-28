cmake_minimum_required(VERSION 4.0)
project(programSphero)

set(CMAKE_CXX_STANDARD 20)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

include(FetchContent)

set(SIMPLE_SOCKET_BUILD_TESTS ON)
FetchContent_Declare(
        SimpleSocket
        GIT_REPOSITORY https://github.com/markaren/SimpleSocket.git
        GIT_TAG master
)


FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)


FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(SimpleSocket)


# Add your executable
add_executable(programSphero main.cpp
        include/server.hpp
        src/server.cpp
        include/vision.hpp
        src/vision.cpp)
# Link OpenCV libraries
target_link_libraries(programSphero ${OpenCV_LIBS})