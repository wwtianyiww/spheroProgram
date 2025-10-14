cmake_minimum_required(VERSION 4.0)
project(programSphero)

set(CMAKE_CXX_STANDARD 20)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Add your executable
add_executable(programSphero main.cpp)
# Link OpenCV libraries
target_link_libraries(programSphero ${OpenCV_LIBS})