#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdint>

#include "JFIF.hpp"
#include "IFD.hpp"
#include "JPEGReader.hpp" 


TEST(ValueConverterTest, SwapUInt16) {
    EXPECT_EQ(valueConverter::swapUInt16(0x1234), 0x3412);
    EXPECT_EQ(valueConverter::swapUInt16(0x0000), 0x0000);
    EXPECT_EQ(valueConverter::swapUInt16(0xFFFF), 0xFFFF);
}

TEST(ValueConverterTest, SwapUInt32) {
    EXPECT_EQ(valueConverter::swapUInt32(0x12345678), 0x78563412);
    EXPECT_EQ(valueConverter::swapUInt32(0x00000000), 0x00000000);
    EXPECT_EQ(valueConverter::swapUInt32(0xFFFFFFFF), 0xFFFFFFFF);
}

TEST(ValueConverterTest, SwapUInt64) {
    EXPECT_EQ(valueConverter::swapUInt64(0x1122334455667788ULL), 0x8877665544332211ULL);
    EXPECT_EQ(valueConverter::swapUInt64(0x0000000000000000ULL), 0x0000000000000000ULL);
    EXPECT_EQ(valueConverter::swapUInt64(0xFFFFFFFFFFFFFFFFULL), 0xFFFFFFFFFFFFFFFFULL);
}

TEST(ValueConverterStreamTest, ReadFromStream) {
    const char testData[] = {
        0x0a,
        0x0b, 0x0c, 
        0x01, 0x02, 0x03, 0x04,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    };
    
    std::string dataStr(testData, sizeof(testData));
    std::stringstream ss(dataStr);
    
    EXPECT_EQ(valueConverter::streamToUInt8(ss),  0x0A);
    EXPECT_EQ(valueConverter::streamToUInt16(ss), 0x0C0B);
    EXPECT_EQ(valueConverter::streamToUInt32(ss), 0x04030201);
    EXPECT_EQ(valueConverter::streamToUInt64(ss), 0x0F0E0D0C0B0A0908);
}

TEST(ValueConverterTest, DispDumbTest) {
    std::vector<std::uint8_t> data = {
        'A', 'B', 'C', 0x00,
        0x10, 0x20, 'a', 'b',
        'c', 'd', 'e', 'f',
        0x01, 0x02, 0x03, 0x04,
        'X', 'Y',               // Fractional data exceeding 16 bytes
    };

    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    valueConverter::dispDumb(data);
    std::cout.rdbuf(oldCout);

    std::string output = buffer.str();

    EXPECT_NE(output.find("Offset    |  +0 +1 +2 +3"), std::string::npos); // header
    EXPECT_NE(output.find("+00000000 |  41 42 43 00"), std::string::npos); // first 16 bytes
    EXPECT_NE(output.find(" | ABC.. abcd"), std::string::npos);           // first 16 bytes in ASCII
    EXPECT_NE(output.find("+00000010 |  58 59"), std::string::npos);
    EXPECT_NE(output.find(" | XY"), std::string::npos);
}

TEST(JPEGReaderTest, readSegment){
    std::vector<std::uint8_t> mock = {
        0xff, 0xd8,             // SOI
        0xff, 0xe0,             // APP0
        0x00, 0x14,
        0x4a, 0x46, 0x49, 0x46, 0x00,       // "JFIF\0"
        0x01, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x2C, 0x00, 0x00, 0x41, 0x4d, 0x50, 0x46,
        0xff, 0xe1,             // APP1
        0x00, 0x12,
        0x45, 0x78, 0x69, 0x66, 0x00, 0x00, // "Exif\0\0"
        0x4d, 0x4d,             // "MM"
        0x00, 0x2a,
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00,
        0xff, 0xd9,             // EOI
    };
    std::vector<std::uint8_t> dtAPP0 = {0x4a, 0x46, 0x49, 0x46, 0x00,       // "JFIF\0"
        0x01, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x2C, 0x00, 0x00, 0x41, 0x4d, 0x50, 0x46};
    std::vector<std::uint8_t> dtAPP1 = {0x45, 0x78, 0x69, 0x66, 0x00, 0x00, // "Exif\0\0"
        0x4d, 0x4d,             // "MM"
        0x00, 0x2a,
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00};

    auto is = mock.begin();
    auto ie = mock.end();
    std::string str(is, ie);
    std::stringstream ss(str);
    std::vector<std::uint8_t> data;

    std:int32_t len = JPEGReader::readSegment(static_cast<std::uint16_t>(enSegmentMarker::APP0), ss, data);
    EXPECT_EQ(dtAPP0, data);

    data.clear();
    len = JPEGReader::readSegment(static_cast<std::uint16_t>(enSegmentMarker::APP1), ss, data);
    EXPECT_EQ(dtAPP1, data);
}


