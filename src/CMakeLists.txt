# Define executable (ONLY here, not in root)
add_executable(programSphero
        ../main.cpp
        server.cpp
        vision.cpp
        autonomousControl.cpp
        ../include/autonomousControl.hpp
        manualControl.cpp
        ../include/manualControl.hpp
        ../include/messageType.hpp
        udpVideoServer.cpp
        ../include/udpVideoServer.hpp
)

add_custom_command(TARGET programSphero POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:programSphero>
        COMMENT "Copying SDL3 DLL to output directory"
)

# Include directories
target_include_directories(programSphero PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${simplesocket_SOURCE_DIR}/include  # SimpleSocket includes
        ${simplesocket_SOURCE_DIR}          # Fallback
        ${OpenCV_INCLUDE_DIRS}
        ${sdl3_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(programSphero PRIVATE
        ${OpenCV_LIBS}
        simple_socket
        SDL3::SDL3
        nlohmann_json::nlohmann_json
)