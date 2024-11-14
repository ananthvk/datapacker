#include "serialization.h"
#include <gtest/gtest.h>

TEST(EncodingOfIntegers, SingleByte)
{
    uint8_t buffer[16] = {0};
    int8_t a = 50;
    uint8_t b = 60;

    encode_be(buffer, a);
    EXPECT_EQ(buffer[0], 50);
    encode_be(buffer, b);
    EXPECT_EQ(buffer[0], 60);

    encode_le(buffer, a);
    EXPECT_EQ(buffer[0], 50);
    encode_le(buffer, b);
    EXPECT_EQ(buffer[0], 60);

    for (auto i = 1; i < 16; ++i)
        EXPECT_EQ(buffer[i], 0);
}

TEST(EncodingOfIntegers, TwoBytes)
{
    int16_t a = 0x7fac;
    uint16_t b = 0x6e5b;
    uint8_t buffer[16] = {0};

    encode_be(buffer, a);
    EXPECT_EQ(buffer[0], 0x7f);
    EXPECT_EQ(buffer[1], 0xac);
    encode_be(buffer, b);
    EXPECT_EQ(buffer[0], 0x6e);
    EXPECT_EQ(buffer[1], 0x5b);

    encode_le(buffer, a);
    EXPECT_EQ(buffer[0], 0xac);
    EXPECT_EQ(buffer[1], 0x7f);
    encode_le(buffer, b);
    EXPECT_EQ(buffer[0], 0x5b);
    EXPECT_EQ(buffer[1], 0x6e);

    for (auto i = 2; i < 16; ++i)
        EXPECT_EQ(buffer[i], 0);
}

TEST(EncodingOfIntegers, FourBytes)
{
    int32_t a = 0x7fabcdef;
    uint32_t b = 0xf0123456;
    uint8_t buffer[16] = {0};

    encode_be(buffer, a);
    EXPECT_EQ(buffer[0], 0x7f);
    EXPECT_EQ(buffer[1], 0xab);
    EXPECT_EQ(buffer[2], 0xcd);
    EXPECT_EQ(buffer[3], 0xef);
    encode_be(buffer, b);
    EXPECT_EQ(buffer[0], 0xf0);
    EXPECT_EQ(buffer[1], 0x12);
    EXPECT_EQ(buffer[2], 0x34);
    EXPECT_EQ(buffer[3], 0x56);

    encode_le(buffer, a);
    EXPECT_EQ(buffer[0], 0xef);
    EXPECT_EQ(buffer[1], 0xcd);
    EXPECT_EQ(buffer[2], 0xab);
    EXPECT_EQ(buffer[3], 0x7f);
    encode_le(buffer, b);
    EXPECT_EQ(buffer[0], 0x56);
    EXPECT_EQ(buffer[1], 0x34);
    EXPECT_EQ(buffer[2], 0x12);
    EXPECT_EQ(buffer[3], 0xf0);

    for (auto i = 4; i < 16; ++i)
        EXPECT_EQ(buffer[i], 0);
}

TEST(EncodingOfIntegers, EightBytes)
{
    int64_t a = 0x7fabcdef01234567;
    uint64_t b = 0xfedcba9876543210;
    uint8_t buffer[16] = {0};

    encode_be(buffer, a);
    EXPECT_EQ(buffer[0], 0x7f);
    EXPECT_EQ(buffer[1], 0xab);
    EXPECT_EQ(buffer[2], 0xcd);
    EXPECT_EQ(buffer[3], 0xef);
    EXPECT_EQ(buffer[4], 0x01);
    EXPECT_EQ(buffer[5], 0x23);
    EXPECT_EQ(buffer[6], 0x45);
    EXPECT_EQ(buffer[7], 0x67);
    encode_be(buffer, b);
    EXPECT_EQ(buffer[0], 0xfe);
    EXPECT_EQ(buffer[1], 0xdc);
    EXPECT_EQ(buffer[2], 0xba);
    EXPECT_EQ(buffer[3], 0x98);
    EXPECT_EQ(buffer[4], 0x76);
    EXPECT_EQ(buffer[5], 0x54);
    EXPECT_EQ(buffer[6], 0x32);
    EXPECT_EQ(buffer[7], 0x10);

    encode_le(buffer, a);
    EXPECT_EQ(buffer[0], 0x67);
    EXPECT_EQ(buffer[1], 0x45);
    EXPECT_EQ(buffer[2], 0x23);
    EXPECT_EQ(buffer[3], 0x01);
    EXPECT_EQ(buffer[4], 0xef);
    EXPECT_EQ(buffer[5], 0xcd);
    EXPECT_EQ(buffer[6], 0xab);
    EXPECT_EQ(buffer[7], 0x7f);
    encode_le(buffer, b);
    EXPECT_EQ(buffer[0], 0x10);
    EXPECT_EQ(buffer[1], 0x32);
    EXPECT_EQ(buffer[2], 0x54);
    EXPECT_EQ(buffer[3], 0x76);
    EXPECT_EQ(buffer[4], 0x98);
    EXPECT_EQ(buffer[5], 0xba);
    EXPECT_EQ(buffer[6], 0xdc);
    EXPECT_EQ(buffer[7], 0xfe);

    for (auto i = 8; i < 16; ++i)
        EXPECT_EQ(buffer[i], 0);
}

TEST(DecodingOfIntegers, SingleByte)
{
    int8_t a = -72, a1;
    uint8_t b = 50, b1;
    uint8_t buffer[16];

    encode_le(buffer, a);
    decode_le(buffer, a1);
    
    encode_le(buffer, b);
    decode_le(buffer, b1);
    
    EXPECT_EQ(a, a1);
    EXPECT_EQ(b, b1);

    for(int8_t i = INT8_MIN; i < INT8_MAX; ++i)
    {
        encode_le(buffer, i);
        decode_le(buffer, a1);
        ASSERT_EQ(i, a1);
    }
    
    for(uint8_t i = 0; i < UINT8_MAX; ++i)
    {
        encode_le(buffer, i);
        decode_le(buffer, b1);
        ASSERT_EQ(i, b1);
    }
    
    a = INT8_MAX;
    b = UINT8_MAX;
    encode_le(buffer, a);
    decode_le(buffer, a1);
    encode_le(buffer, b);
    decode_le(buffer, b1);
    ASSERT_EQ(a, a1);
    ASSERT_EQ(b, b1);
}

TEST(DecodingOfIntegers, TwoBytes)
{
    int16_t a = -0x7fac, a1;
    uint16_t b = 0x6e5b, b1;
    uint8_t buffer[16] = {0};
    
    encode_le(buffer, a);
    decode_le(buffer, a1);
    encode_be(buffer, b);
    decode_be(buffer, b1);
    
    ASSERT_EQ(a, a1);
    ASSERT_EQ(b, b1);
}

TEST(DecodingOfIntegers, FourBytes)
{
    int32_t a = -0x7fabcdef, a1;
    uint32_t b = 0xf0123456, b1;
    uint8_t buffer[16] = {0};

    encode_le(buffer, a);
    decode_le(buffer, a1);
    encode_be(buffer, b);
    decode_be(buffer, b1);
    
    ASSERT_EQ(a, a1);
    ASSERT_EQ(b, b1);
}

TEST(DecodingOfIntegers, EightBytes)
{
    int64_t a = -0x7fabcdef01234567, a1;
    uint64_t b = 0xfedcba9876543210, b1;
    uint8_t buffer[16] = {0};

    encode_le(buffer, a);
    decode_le(buffer, a1);
    encode_be(buffer, b);
    decode_be(buffer, b1);
    
    ASSERT_EQ(a, a1);
    ASSERT_EQ(b, b1);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}