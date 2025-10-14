#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Print OpenCV version
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;

    // Open the default camera (camera 0)
    cv::VideoCapture camera(0);

    // Check if camera opened successfully
    if (!camera.isOpened()) {
        std::cerr << "ERROR! Unable to open camera" << std::endl;
        return -1;
    }

    std::cout << "Camera opened successfully!" << std::endl;
    std::cout << "Press 'q' to quit" << std::endl;

    cv::Mat frame;

    // Main loop - capture and display frames
    while (true) {
        // Capture a frame
        camera >> frame;

        // Check if frame is empty
        if (frame.empty()) {
            std::cerr << "ERROR! Blank frame grabbed" << std::endl;
            break;
        }

        // Optional: Add text overlay on the video
        cv::putText(frame, "Webcam Feed - Press 'q' to quit",
                    cv::Point(10, 30),
                    cv::FONT_HERSHEY_SIMPLEX, 0.7,
                    cv::Scalar(0, 255, 0), 2);

        // Display the frame
        cv::imshow("Webcam", frame);

        // Wait for 1ms and check if 'q' key is pressed
        if (cv::waitKey(1) == 'q') {
            std::cout << "Quitting..." << std::endl;
            break;
        }
    }

    // Release the camera and close windows
    camera.release();
    cv::destroyAllWindows();

    std::cout << "Camera test completed!" << std::endl;

    return 0;
}