#include "../include/udpVideoServer.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>



UDPVideoServer::UDPVideoServer(uint16_t port) : udpSocket(port), port(port) {
    std::cout << "UDP video server initialized on port " << port << std::endl;
}

UDPVideoServer::~UDPVideoServer() {
    std::cout << "UDP video server closed" << std::endl;
}

cv::Mat UDPVideoServer::recv() {
    try {
        // Receive UDP packet (blocking call)
        // Using recvFrom with "0.0.0.0" and port 0 to receive from any sender
        std::vector<uint8_t> buffer(65536); // 64KB max practical UDP packet size
        int bytesReceived = udpSocket.recvFrom("0.0.0.0", 0, buffer);

        if (bytesReceived <= 0) {
            return cv::Mat(); // Return empty Mat if no data
        }

        // Resize buffer to actual received size
        buffer.resize(bytesReceived);

        // Decode JPEG to cv::Mat
        cv::Mat frame = cv::imdecode(buffer, cv::IMREAD_COLOR);

        if (frame.empty()) {
            std::cerr << "Failed to decode frame (" << bytesReceived << " bytes)" << std::endl;
        }

        return frame;

    } catch (const std::exception& e) {
        std::cerr << "UDP receive error: " << e.what() << std::endl;
        return cv::Mat();
    }
}


bool UDPVideoServer::isReceiving() {
     return true;
}

void UDPVideoServer::recvLoop() {
    // Continuous receive loop
    while (isReceiving()) {
        cv::Mat frame = recv();
        if (!frame.empty()) {
        }
    }
}

