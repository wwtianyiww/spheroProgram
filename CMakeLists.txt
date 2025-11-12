cmake_minimum_required(VERSION 4.0)
project(programSphero)

set(CMAKE_CXX_STANDARD 20)

# Find OpenCV package
find_package(OpenCV REQUIRED)

include(FetchContent)

set(SIMPLE_SOCKET_BUILD_TESTS OFF)

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


FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.26
)

# from claude: SDL3 options (optional but recommended)
set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3)
FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(SimpleSocket)

# DEBUG: Find SimpleSocket headers
FetchContent_GetProperties(SimpleSocket)
message(STATUS "========================================")
message(STATUS "SimpleSocket SOURCE: ${simplesocket_SOURCE_DIR}")
file(GLOB_RECURSE SOCKET_HEADERS "${simplesocket_SOURCE_DIR}/*.hpp")
foreach(header ${SOCKET_HEADERS})
    message(STATUS "  ${header}")
endforeach()
message(STATUS "========================================")

# Add the src subdirectory (executable defined there)
add_subdirectory(src)