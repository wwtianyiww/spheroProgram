#include "server.hpp"
#include <simple_socket/TCPSocket.hpp>
#include <simple_socket/SimpleConnection.hpp>
#include <nlohmann/json.hpp>
#include <atomic>
#include <iostream>
#include <string>
#include <thread>

uint32_t Server::readU32(simple_socket::SimpleConnection &connection) {
    uint8_t buffer[4]; //array to store the 4 bytes of 8 bits
    connection.read(buffer, 4);
    return (uint32_t(buffer[0]) << 24) | (uint32_t(buffer[1]) << 16) | (uint32_t(buffer[2]) << 8) | uint32_t(buffer[3]); //shift and combine
}

void Server::writeU32(simple_socket::SimpleConnection &connection, uint32_t length) {
    uint8_t buffer[4] = { uint8_t(length >> 24), uint8_t(length >> 16), uint8_t(length >> 8), uint8_t(length) }; //separate bytes and shift
    connection.write(buffer, 4);
}

std::vector<uint8_t> Server::getFrames(simple_socket::SimpleConnection &connection) {
    const uint32_t size = readU32(connection); //get size of message
    std::vector<uint8_t> data(size);
    if (size) {
        connection.read(data.data(), size); //read data of size
    }
    return data;
}

void Server::sendFrame(simple_socket::SimpleConnection &connection, const std::vector<uint8_t> &payload) {
    writeU32(connection, static_cast<uint32_t>(payload.size())); //size -> big endian
    if (!payload.empty()) {
        connection.write(payload.data(), payload.size()); //send payload of size
    }
}

void Server::sendTypedFrame(simple_socket::SimpleConnection &connection, messageProtocol::MessageType type,
    const std::vector<uint8_t> &payload) {
    std::vector<uint8_t> message;
    message.reserve(1 + payload.size());

    message.push_back(static_cast<uint8_t>(type));

    message.insert(message.end(), payload.begin(), payload.end());
    sendFrame(connection, message);
}

std::pair<messageProtocol::MessageType, std::vector<uint8_t>> Server::receiveTypedFrame(
    simple_socket::SimpleConnection &connection) {
    std::vector<uint8_t> message = getFrames(connection);

    if (message.empty()) {
        throw std::runtime_error("Empty frame");
    }

    messageProtocol::MessageType type = static_cast<messageProtocol::MessageType>(message[0]);
    std::vector<uint8_t> payload(message.begin() +1, message.end());

    return {type, payload};
}


//recieve video data, sensor data and robot state
void Server::handleClient(std::unique_ptr<simple_socket::SimpleConnection> connection, int id) {

}

/*
simple_socket::TCPServer Server::server(int port) {
    server(port);
}
*/