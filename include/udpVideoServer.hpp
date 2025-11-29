#ifndef PROGRAMSPHERO_UDPVIDEOSERVER_HPP
#define PROGRAMSPHERO_UDPVIDEOSERVER_HPP
#include <opencv2/core/mat.hpp>
#include <simple_socket/UDPSocket.hpp>


class UDPVideoServer {
    public:
    explicit UDPVideoServer(uint16_t port);

    ~UDPVideoServer();

    cv::Mat recv();

    bool isReceiving();
    private:
    void recvLoop();

    simple_socket::UDPSocket udpSocket;

    uint16_t port;


};


#endif //PROGRAMSPHERO_UDPVIDEOSERVER_HPP