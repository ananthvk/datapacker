#include <stddef.h>
#include <stdint.h>
#include <type_traits>

template <typename T> inline void encode_be(uint8_t *buffer, T value)
{
    if constexpr (std::is_integral<T>::value)
    {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);
        buffer[0] = static_cast<uint8_t>((value >> (8 * sizeof(T) - 8)) & 0xFF);
        if constexpr (sizeof(T) >= 2)
        {
            buffer[1] = static_cast<uint8_t>((value >> (8 * sizeof(T) - 16)) & 0xFF);
        }
        if constexpr (sizeof(T) >= 4)
        {
            buffer[2] = static_cast<uint8_t>((value >> (8 * sizeof(T) - 24)) & 0xFF);
            buffer[3] = static_cast<uint8_t>((value >> (8 * sizeof(T) - 32)) & 0xFF);
        }
        if constexpr (sizeof(T) >= 8)
        {
            buffer[4] = static_cast<uint8_t>((value >> (8 * sizeof(T) - 40)) & 0xFF);
            buffer[5] = static_cast<uint8_t>((value >> (8 * sizeof(T) - 48)) & 0xFF);
            buffer[6] = static_cast<uint8_t>((value >> (8 * sizeof(T) - 56)) & 0xFF);
            buffer[7] = static_cast<uint8_t>((value >> (8 * sizeof(T) - 64)) & 0xFF);
        }
    }
}

template <typename T> inline void encode_le(uint8_t *buffer, T value)
{
    if constexpr (std::is_integral<T>::value)
    {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);
        buffer[0] = static_cast<uint8_t>(value & 0xFF);
        if constexpr (sizeof(T) >= 2)
        {
            buffer[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
        }
        if constexpr (sizeof(T) >= 4)
        {
            buffer[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
            buffer[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
        }
        if constexpr (sizeof(T) >= 8)
        {
            buffer[4] = static_cast<uint8_t>((value >> 32) & 0xFF);
            buffer[5] = static_cast<uint8_t>((value >> 40) & 0xFF);
            buffer[6] = static_cast<uint8_t>((value >> 48) & 0xFF);
            buffer[7] = static_cast<uint8_t>((value >> 56) & 0xFF);
        }
    }
}

template <typename T> inline void decode_le(uint8_t *buffer, T &value)
{
    if constexpr (std::is_integral<T>::value)
    {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);
        // Perform the decoding in unsigned type only, then convert it to the type of T
        using uT = std::make_unsigned_t<T>;
        uT val = 0;

        val |= static_cast<uT>(buffer[0]);
        if constexpr (sizeof(T) >= 2)
        {
            val |= static_cast<uT>(static_cast<uT>(buffer[1]) << 8);
        }
        if constexpr (sizeof(T) >= 4)
        {
            val |= static_cast<uT>(static_cast<uT>(buffer[2]) << 16);
            val |= static_cast<uT>(static_cast<uT>(buffer[3]) << 24);
        }
        if constexpr (sizeof(T) >= 8)
        {
            val |= static_cast<uT>(static_cast<uT>(buffer[4]) << 32);
            val |= static_cast<uT>(static_cast<uT>(buffer[5]) << 40);
            val |= static_cast<uT>(static_cast<uT>(buffer[6]) << 48);
            val |= static_cast<uT>(static_cast<uT>(buffer[7]) << 56);
        }
        value = static_cast<T>(val);
    }
}

template <typename T> inline void decode_be(uint8_t *buffer, T &value)
{
    if constexpr (std::is_integral<T>::value)
    {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8);
        // Perform the decoding in unsigned type only, then convert it to the type of T
        using uT = std::make_unsigned_t<T>;
        uT val = 0;

        val |= static_cast<uT>(buffer[0]);
        if constexpr (sizeof(T) >= 2)
        {
            val <<= 8;
            val |= static_cast<uT>(buffer[1]);
        }
        if constexpr (sizeof(T) >= 4)
        {
            val <<= 8;
            val |= static_cast<uT>(buffer[2]);

            val <<= 8;
            val |= static_cast<uT>(buffer[3]);
        }
        if constexpr (sizeof(T) >= 8)
        {
            val <<= 8;
            val |= static_cast<uT>(buffer[4]);

            val <<= 8;
            val |= static_cast<uT>(buffer[5]);

            val <<= 8;
            val |= static_cast<uT>(buffer[6]);

            val <<= 8;
            val |= static_cast<uT>(buffer[7]);
        }
        value = static_cast<T>(val);
    }
}

void encode_float_le(float value, uint8_t *buffer) {}

void encode_double_le(double value, uint8_t *buffer) {}