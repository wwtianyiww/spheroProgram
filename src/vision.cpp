#include "vision.hpp"

#include <WinSock2.h>


Vision::Vision() {
}

Vision::~Vision() {
}

void Vision::initialize() {
    cv::VideoCapture camera(0);

    if (!camera.isOpened()) {
        std::cerr << "ERROR! Unable to open camera" << std::endl;
        return;
    }

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

        segment(frame);
        // Display the frame
        cv::imshow("Webcam", frame);

        // Wait for 1ms and check if 'q' key is pressed
        if (cv::waitKey(1) == 'q') {
            std::cout << "Quitting..." << std::endl;
            camera.release();
            cv::destroyAllWindows();
            //break;
        }
    }
}

void Vision::segment(cv::Mat &frame) {

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);


    cv::Mat binary;
    cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);


    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(binary, binary, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);


    cv::Mat sure_bg;
    cv::dilate(binary, sure_bg, kernel, cv::Point(-1, -1), 3);


    cv::Mat dist_transform;
    cv::distanceTransform(binary, dist_transform, cv::DIST_L2, 5);

    cv::Mat sure_fg;
    double maxVal;
    cv::minMaxLoc(dist_transform, nullptr, &maxVal);
    cv::threshold(dist_transform, sure_fg, 0.7 * maxVal, 255, cv::THRESH_BINARY);
    sure_fg.convertTo(sure_fg, CV_8U);


    cv::Mat unknown;
    cv::subtract(sure_bg, sure_fg, unknown);


    cv::Mat markers;
    cv::connectedComponents(sure_fg, markers);
    markers = markers + 1;  // Background is not 0, but 1


    for (int i = 0; i < markers.rows; i++) {
        for (int j = 0; j < markers.cols; j++) {
            if (unknown.at<uchar>(i, j) == 255) {
                markers.at<int>(i, j) = 0;
            }
        }
    }


    cv::watershed(frame, markers);


    for (int i = 0; i < markers.rows; i++) {
        for (int j = 0; j < markers.cols; j++) {
            if (markers.at<int>(i, j) == -1) {
                frame.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);  // Red boundary
            }
        }
    }
}


/*
void shutdown(cv::VideoCapture camera) {
    camera.release();
    cv::destroyAllWindows();

    std::cout << "Camera test completed!" << std::endl;
}
*/
/*
void Vision::processFrame(const cv::Mat& frame) {
}

std::vector<Vision::DetectionResult> Vision::getDetections() const {
}

void Vision::setFrameCallback(FrameCallback callback) {
}

void Vision::setDetectionCallback(DetectionCallback callback) {
}



cv::Mat Vision::preprocessImage(const cv::Mat &image) {
}

*/

/*
void Vision::thresholding() {
    cv::cvtColor();
    cv::threshold();
}
*/

