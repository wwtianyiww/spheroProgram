#include "server.hpp"
#include <simple_socket/TCPSocket.hpp>
#include <simple_socket/SimpleConnection.hpp>
#include <nlohmann/json.hpp>
#include <atomic>
#include <iostream>
#include <vector>
#include <optional>
#include <string>
#include <thread>

using nlohmann::json;

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
    const uint32_t size = readU32(connection); //size of message
    std::vector<uint8_t> data(size); //vector of appropriate size
    if (size) {
        connection.read(data.data(), size); //read data of size
    }
    return data;
}

void Server::sendFrame(simple_socket::SimpleConnection &connection, const std::vector<uint8_t> &payload) {
    writeU32(connection, static_cast<uint32_t>(payload.size())); //size in 4bytes (32 bits)
    if (!payload.empty()) {
        connection.write(payload.data(), payload.size()); //send payload of size
    }
}

void Server::sendTypedFrame(simple_socket::SimpleConnection &connection, messageProtocol::MessageType type,
    const std::vector<uint8_t> &payload) {
    std::vector<uint8_t> message;
    message.reserve(1 + payload.size()); //adds one byte for "type"

    message.push_back(static_cast<uint8_t>(type));

    message.insert(message.end(), payload.begin(), payload.end());
    sendFrame(connection, message); //
}

std::optional<std::pair<messageProtocol::MessageType, std::vector<uint8_t>>> Server::receiveTypedFrame(
    simple_socket::SimpleConnection &connection) {
    std::vector<uint8_t> message = getFrames(connection);

    if (message.empty()) {
        throw std::runtime_error("Empty frame");
    }

    messageProtocol::MessageType type = static_cast<messageProtocol::MessageType>(message[0]);
    std::vector<uint8_t> payload(message.begin() +1, message.end());

    return std::make_pair(type, payload);
}


static std::vector<uint8_t> jsonToText(const json& j) {
    std::string s = j.dump(2); //convert to readable json
    return {s.begin(), s.end()}; //characters of s
}

//to msgpack (text)
static std::vector<uint8_t> jsonToMsgPack(const json& j) {
    std::vector<uint8_t> out;
    json::to_msgpack(j, out); // MessagePack serialization of json
    return out;
}

//bytes to json (binary)
static bool parseJson(const std::vector<uint8_t>& buf, json& out) {
    try {
        out = json::parse(std::string(buf.begin(), buf.end()));
        return true;
    }
    catch (...) {
        return false;
    }
}

//msgpack to json
static bool parseMsgPack(const std::vector<uint8_t>& buf, json& out) {
    try {
        out = json::from_msgpack(buf);
        return true;
    }
    catch (...) {
        return false;
    }
}



//recieve video data, sensor data and robot state
void Server::handleClient(const std::unique_ptr<simple_socket::SimpleConnection> &connection) {
    try {
        std::cout << "Client Connected" << std::endl;
        for (;;) {
            auto frame = receiveTypedFrame(*connection);
            if (!frame.has_value()) {
                std::cout << "Empty Payload" << std::endl;
                break;
            }

            auto [type, payload] = frame.value();
            json j;

            if (parseJson(payload, j)) {
                //recieving json
                std::cout << "[client] got json:\n" << j.dump(2) << "\n";
                auto bin = jsonToMsgPack(j);
                std::cout << "[client] send MessagePack (" << bin.size() << " bytes)\n";
                sendFrame(*connection, bin);
            } else if (parseMsgPack(payload, j)) {
                // recieving binary
                std::cout << "[client] got MessagePack (" << payload.size() << " bytes)\n"
                          << "as JSON:\n" << j.dump(2) << "\n";
                auto txt = jsonToText(j);
                std::cout << "[client] send JSON (" << txt.size() << " bytes)\n";
                sendFrame(*connection, txt);
            } else {
                std::cout << "[client] unsupported payload\n";
                break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Client handler error: " << e.what() << std::endl;
    }
}

/*
simple_socket::TCPServer Server::server(int port) {
    server(port);
}
*/