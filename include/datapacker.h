/**
 * @file datapacker.h
 * @brief Utilities for encoding/decoding different data types in various binary formats
 *
 * This file contains various template functions which can be used to encode/decode data types in a
 * portable way. Contains functions to encode/decode little-endian/big-endian, floats (using IEEE754
 * format), strings and arrays.
 * @note
 * - Ensure that buffer has enough length to store all the values to prevent buffer overflows
 * @version 1.0
 * @date 2024-11-14
 * @author Ananthanarayanan Venkitakrishnan
 */
#ifndef A_DATAPACKER_H
#define A_DATAPACKER_H
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <type_traits>

namespace datapacker
{
namespace internal
{
template <typename T> struct is_float : std::false_type
{
};

template <typename T> struct is_double : std::false_type
{
};

template <> struct is_float<float> : std::true_type
{
};

template <> struct is_double<double> : std::true_type
{
};

template <unsigned bits, unsigned expbits> uint64_t pack754(long double f);

template <unsigned bits, unsigned expbits> long double unpack754(uint64_t i);
} // namespace internal

/**
 * @brief Encodes a float in IEEE754 format and stores it in `buffer`
 * @param buffer Pointer to buffer which will be used to store the encoded data
 * @param value Value to be encoded
 * @return Number of bytes written to the buffer
 * @note `buffer` should have size atleast equal to `sizeof(float)`
 */
void encode_float(uint8_t *buffer, float f)
{
    uint64_t encoded = internal::pack754<32, 8>(f);
    uint32_t result = static_cast<uint32_t>(encoded & 0xFFFFFFFF);
    memcpy(buffer, &result, sizeof(result));
}

/**
 * @brief Encodes a double in IEEE754 format and stores it in `buffer`
 * @param buffer Pointer to buffer which will be used to store the encoded data
 * @param value Value to be encoded
 * @return Number of bytes written to the buffer
 * @note `buffer` should have size atleast equal to `sizeof(double)`
 */
void encode_double(uint8_t *buffer, double f)
{
    uint64_t encoded = internal::pack754<64, 11>(f);
    memcpy(buffer, &encoded, sizeof(encoded));
}

/**
 * @brief Decodes a float stored in IEEE754 format and stores it in `f`
 * @param buffer Pointer to buffer which contains the encoded data
 * @param value Where the decoded value will be stored
 * @return Number of bytes read from the buffer
 * @note `buffer` should have size atleast equal to `sizeof(float)`
 */
void decode_float(uint8_t *buffer, float &f)
{
    uint64_t i;
    memcpy(&i, buffer, sizeof(float));
    f = static_cast<float>(internal::unpack754<32, 8>(i));
}

/**
 * @brief Decodes a double stored in IEEE754 format and stores it in `f`
 * @param buffer Pointer to buffer which contains the encoded data
 * @param value Where the decoded value will be stored
 * @return Number of bytes read from the buffer
 * @note `buffer` should have size atleast equal to `sizeof(double)`
 */
void decode_double(uint8_t *buffer, double &f)
{
    uint64_t i;
    memcpy(&i, buffer, sizeof(double));
    f = static_cast<double>(internal::unpack754<64, 11>(i));
}

/**
 * @brief Encodes a value in big-endian format and stores it in `buffer`
 * @tparam T Type of value to be encoded, `sizeof(T)` bytes are written to the buffer
 * @param buffer Pointer to buffer which will be used to store the encoded data
 * @param value Value to be encoded
 * @return Number of bytes written to the buffer
 * @note `buffer` should have size atleast equal to `sizeof(T)`
 *
 * Example usage:
 * @code
 * uint8_t buffer[4];
 * int x = 32;
 * encode_be(buffer, x);
 * @endcode
 */
template <typename T> inline void encode_be(uint8_t *buffer, T value)
{
    static_assert(std::is_integral<T>::value);
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

/**
 * @brief Encodes a value in little-endian format and stores it in `buffer`
 * @tparam T Type of value to be encoded, `sizeof(T)` bytes are written to the buffer
 * @param buffer Pointer to buffer which will be used to store the encoded data
 * @param value Value to be encoded
 * @return Number of bytes written to the buffer
 * @note `buffer` should have size atleast equal to `sizeof(T)`
 *
 * Example usage:
 * @code
 * uint8_t buffer[4];
 * int x = 32;
 * encode_le(buffer, x);
 * @endcode
 */
template <typename T> inline void encode_le(uint8_t *buffer, T value)
{
    static_assert(std::is_integral<T>::value);
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

/**
 * @brief Decodes a little-endian encoded value from the buffer and stores it in `value`, the number
 * of bytes processed depends upon the type `T` of value passed
 * @tparam T Type of value to be decoded, `sizeof(T)` bytes are read from the buffer
 * @param buffer Pointer to buffer which contains the encoded data
 * @param value Reference to a variable of type `T`, which will hold the decoded value
 * @return Number of bytes read from the buffer
 * @note `buffer` should have size atleast equal to `sizeof(T)`
 *
 * Example usage:
 * @code
 * // Assume buffer has a little endian encoded value
 * uint8_t buffer[4];
 * int x;
 * decode_le(buffer, x);
 * @endcode
 */
template <typename T> inline void decode_le(uint8_t *buffer, T &value)
{
    static_assert(std::is_integral<T>::value);
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

/**
 * @brief Decodes a big-endian encoded value from the buffer and stores it in `value`, the number of
 * bytes processed depends upon the type `T` of value passed
 * @tparam T Type of value to be decoded, `sizeof(T)` bytes are read from the buffer
 * @param buffer Pointer to buffer which contains the encoded data
 * @param value Reference to a variable of type `T`, which will hold the decoded value
 * @return Number of bytes read from the buffer
 * @note `buffer` should have size atleast equal to `sizeof(T)`
 *
 * Example usage:
 * @code
 * // Assume buffer has a big endian encoded value
 * uint8_t buffer[4];
 * int x;
 * decode_be(buffer, x);
 * @endcode
 */
template <typename T> inline void decode_be(uint8_t *buffer, T &value)
{
    static_assert(std::is_integral<T>::value);
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

/**
 * @brief Encodes multiple values in little-endian format and then stores them in a buffer
 * @tparam T Type of the first parameter to encode
 * @tparam Args Type of additional values to pack into the buffer
 * @param buffer Pointer to the buffer where the encoded values will be written to
 * @param value The first value to encode
 * @param Args Additional values to pack
 * @return Number of bytes written to the buffer
 * @note `buffer` should be of sufficient length to prevent overflow, i.e. it should be atleast
 * equal to the sum of sizes of the types passed
 */
template <typename T, typename... Args> void encode_le(uint8_t *buffer, T value, Args... args)
{
    encode_le(buffer, value);
    encode_le(buffer + sizeof(T), args...);
}

/**
 * @brief Encodes multiple values in big-endian format and then stores them in a buffer
 * @tparam T Type of the first parameter to encode
 * @tparam Args Type of additional values to pack into the buffer
 * @param buffer Pointer to the buffer where the encoded values will be written to
 * @param value The first value to encode
 * @param Args Additional values to pack
 * @return Number of bytes written to the buffer
 * @note `buffer` should be of sufficient length to prevent overflow, i.e. it should be atleast
 * equal to the sum of sizes of the types passed
 */
template <typename T, typename... Args> void encode_be(uint8_t *buffer, T value, Args... args)
{
    encode_be(buffer, value);
    encode_be(buffer + sizeof(T), args...);
}

/**
 * @brief Decodes multiple values in little-endian format and stores the decoded value in the passed
 * references
 * @tparam T Type of the first parameter to decode
 * @tparam Args Type of additional values to unpack from the buffer
 * @param buffer Pointer to the buffer where the encoded values are present
 * @param value The first value to decode
 * @param Args Additional values to unpack
 * @return Number of bytes processed from the buffer
 * @note `buffer` should be of sufficient length to prevent overflow, i.e. it should be atleast
 * equal to the sum of sizes of the types passed
 */
template <typename T, typename... Args> void decode_le(uint8_t *buffer, T &value, Args &...args)
{
    decode_le(buffer, value);
    decode_le(buffer + sizeof(T), args...);
}

/**
 * @brief Decodes multiple values in big-endian format and stores the decoded value in the passed
 * references
 * @tparam T Type of the first parameter to decode
 * @tparam Args Type of additional values to unpack from the buffer
 * @param buffer Pointer to the buffer where the encoded values are present
 * @param value The first value to decode
 * @param Args Additional values to unpack, each value will be decoded from the appropriate offset
 * in the buffer
 * @return Number of bytes processed from the buffer
 * @note `buffer` should be of sufficient length to prevent overflow, i.e. it should be atleast
 * equal to the sum of sizes of the types passed
 */
template <typename T, typename... Args> void decode_be(uint8_t *buffer, T &value, Args &...args)
{
    decode_be(buffer, value);
    decode_be(buffer + sizeof(T), args...);
}

namespace internal
{
// Code taken from https://beej.us/guide/bgnet/source/examples/ieee754.c

/**
 * This function packs a double as an unsigned 64 bit integer, which can be written to a buffer
 */
template <unsigned bits, unsigned expbits> inline uint64_t pack754(long double f)
{
    long double fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (f == 0.0)
        return 0; // get this special case out of the way

    // check sign and begin normalization
    if (f < 0)
    {
        sign = 1;
        fnorm = -f;
    }
    else
    {
        sign = 0;
        fnorm = f;
    }

    // get the normalized form of f and track the exponent
    shift = 0;
    while (fnorm >= 2.0)
    {
        fnorm /= 2.0;
        shift++;
    }
    while (fnorm < 1.0)
    {
        fnorm *= 2.0;
        shift--;
    }
    fnorm = fnorm - 1.0;

    // calculate the binary form (non-float) of the significand data
    significand =
        static_cast<long long>(fnorm * (static_cast<float>(1LL << significandbits) + 0.5f));

    // get the biased exponent
    exp = shift + ((1 << (expbits - 1)) - 1); // shift + bias

    // return the final answer
    return static_cast<uint64_t>((sign << (bits - 1)) | (exp << (bits - expbits - 1)) |
                                 significand);
}

/**
 * This function unpacks a unsigned 64 bit integer into a double
 */
template <unsigned bits, unsigned expbits> inline long double unpack754(uint64_t i)
{
    long double result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    if (i == 0)
        return 0.0;

    // pull the significand
    result = (i & ((1LL << significandbits) - 1)); // mask
    result /= (1LL << significandbits);            // convert back to float
    result += 1.0f;                                // add the one back on

    // deal with the exponent
    bias = static_cast<unsigned int>((1 << (expbits - 1)) - 1);
    shift = static_cast<long long>(((i >> significandbits) & ((1LL << expbits) - 1)) - bias);
    while (shift > 0)
    {
        result *= 2.0;
        shift--;
    }
    while (shift < 0)
    {
        result /= 2.0;
        shift++;
    }

    // sign it
    result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;

    return result;
}
} // namespace internal
} // namespace datapacker
#endif // A_DATAPACKER_H