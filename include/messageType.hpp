#ifndef PROGRAMSPHERO_MESSAGETYPE_HPP
#define PROGRAMSPHERO_MESSAGETYPE_HPP
#include <cstdint>

namespace messageProtocol {
    enum class MessageType : uint8_t {
        IMAGE_FRAME = 0x01,
        FRONT_SENSOR_DATA = 0x02,
        REAR_SENSOR_DATA = 0x03,
        CONTROL_COMMAND = 0x04,
        HEARTBEAT = 0x05
    };
}

#endif //PROGRAMSPHERO_MESSAGETYPE_HPP