#ifndef PROGRAMSPHERO_SERVER_HPP
#define PROGRAMSPHERO_SERVER_HPP
#include <nlohmann/json.hpp>
#include <atomic>
#include <cstdint>
#include <iostream>
#include <string>
#include <messageType.hpp>
#include <thread>
#include <vector>
#include <utility>

namespace simple_socket {
    class SimpleConnection;
    class TCPServer;
}


class Server {


    public:
    static uint32_t readU32(simple_socket::SimpleConnection& connection);
    static void writeU32(simple_socket::SimpleConnection& connection, uint32_t length);

    static std::vector<uint8_t> getFrames(simple_socket::SimpleConnection& connection);
    static void sendFrame(simple_socket::SimpleConnection& connection,  const std::vector<uint8_t>& payload);

    static void sendTypedFrame(simple_socket::SimpleConnection& connection, messageProtocol::MessageType type,  const std::vector<uint8_t>& payload);

    static std::optional<std::pair<messageProtocol::MessageType, std::vector<uint8_t>>>
        receiveTypedFrame(simple_socket::SimpleConnection& connection);

    static void handleClient(const std::unique_ptr<simple_socket::SimpleConnection> &connection);
    simple_socket::TCPServer server(int port);



    private:

};





#endif //PROGRAMSPHERO_SERVER_HPP