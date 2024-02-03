#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <cstring>

namespace message
{
    enum class MessageType
    {
        MOVERS,
        MAP,
        BULLETS
    };

    struct MessageHeader
    {
        MessageType type;
        size_t size = 0;
    };

    struct Message
    {
        MessageHeader header;
        std::vector<uint8_t> body;

        template <typename T>
        void add(const T& data)
        {
            const uint8_t* byteData = reinterpret_cast<const uint8_t*>(&data);
            body.insert(body.end(), byteData, byteData + sizeof(T));
            header.size += sizeof(T);
        }

        template <typename T>
        T get()
        {
            if (body.size() < sizeof(T))
                throw std::runtime_error("Not enough data in the message body to retrieve type T");

            T result;
            std::memcpy(&result, &body[body.size() - sizeof(T)], sizeof(T));
            body.resize(body.size() - sizeof(T));
            header.size -= sizeof(T);
            return result;
        }
    };
}
