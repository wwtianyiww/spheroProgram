#include <opencv2/opencv.hpp>
#include <iostream>

#include <server.hpp>
#include <vision.hpp>
#include <manualControl.hpp>

int main() {
    std::cout << "\n=== Xbox Controller Test ===" << std::endl;

    ManualControl controller;

    if (!controller.init()) {
        std::cerr << "Failed to initialize controller!" << std::endl;
        return 1;
    }

    std::cout << "\nController ready! Move sticks and press buttons..." << std::endl;
    std::cout << "Press Ctrl+C to exit\n" << std::endl;

    // Test loop
    int counter = 0;
    while (controller.isRunning()) {
        controller.update();

        // Print every 10 frames (about 3 times per second)
        if (counter++ % 10 == 0) {
            controller.printState();
            std::cout << std::endl; // New line for clarity
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    return 0;

    /*
    Vision vision;
    vision.initialize();

    return 0;
    */
}