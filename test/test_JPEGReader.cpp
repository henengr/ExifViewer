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

TEST(JPEGReaderTest, ReadStreamSegments) {
    std::vector<std::uint8_t> mockJpeg = {
        0xff, 0xd8,
        0x00, 0x0a,
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    };

    std::string dataStr(mockJpeg.begin(), mockJpeg.end());
    std::stringstream ss(dataStr);
    JPEGReader reader(ss);

    std::uint16_t marker = 0;
    EXPECT_TRUE(reader.getMarker(marker));
    EXPECT_EQ(marker, 0xFFD8);

    std::uint16_t length = 0;
    EXPECT_TRUE(reader.getLength(length));
    EXPECT_EQ(length, 10);

    std::vector<std::uint8_t> segmentData;
    EXPECT_TRUE(reader.readSegment(length, segmentData));
    ASSERT_EQ(segmentData.size(), 8);
    EXPECT_EQ(static_cast<uint8_t>(segmentData[0]), 0x11);
    EXPECT_EQ(static_cast<uint8_t>(segmentData[7]), 0x88);
}

TEST(JPEGReaderTest, GetAPP0Test) {
    std::vector<std::uint8_t> mockDataShort = {
        'J', 'F', 'I', 'F', 0x00, // Identifier
        0x01, 0x02,               // Version
        0x01,                     // PixelConcentrationUnit
    };
    std::vector<std::uint8_t> mockDataNoJFIF = {
        'A', 'B', 'C', 'D', 'E',
        0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData = {
        'J', 'F', 'I', 'F', 0x00, // Identifier
        0x01, 0x02,               // Version
        0x01,                     // PixelConcentrationUnit
        0x2c, 0x01,               // PixelConcentrationX
        0x2c, 0x01,               // PixelConcentrationY
        0x4e, 0x4e,
    };

    std::stringstream ss;
    JPEGReader reader(ss);

    stAPP0 app0{};
    EXPECT_FALSE(reader.getAPP0(mockDataShort.size(), app0, mockDataShort));    // Abnormal scenario
    EXPECT_FALSE(reader.getAPP0(mockDataNoJFIF.size(), app0, mockDataNoJFIF));  // No JFIF

    EXPECT_TRUE(reader.getAPP0(mockData.size(), app0, mockData));
    EXPECT_STREQ(reinterpret_cast<const char*>(app0.JFIFIdentifier), "JFIF");
    EXPECT_EQ(app0.version, 0x0201);    // Version
    EXPECT_EQ(app0.pixelConcentrationUnit, 0x01);   // PixelConcentrationUnit
    EXPECT_EQ(app0.pixelConcentrationX, 0x012C);   // PixelConcentrationX
    EXPECT_EQ(app0.pixelConcentrationY, 0x012C);   // PixelConcentrationY
}

TEST(JPEGReaderTest, GetAPP1Test) {
    std::vector<std::uint8_t> mockDataShort = {
        'E', 'x', 'i', 'f', 0x00, 0x00, // ExifIdentifier (6bytes)
        'M', 'M',                       // TIFFByteOrder -> Big Endian check
    };
    std::vector<std::uint8_t> mockDataNoExif = {
        'A', 'B', 'C', 'D', 'E', 'F',
        0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataBE = {
        'E', 'x', 'i', 'f', 0x00, 0x00, // ExifIdentifier (6bytes)
        'M', 'M',                       // TIFFByteOrder -> Big Endian check
        0x00, 0x2a,                     // TIFFIdentifier (2bytes)
        0x00, 0x00, 0x00, 0x08,         // IFD0thOffset (4bytes)
        0xaa, 0xbb, 0xcc,
    };
    std::vector<std::uint8_t> mockDataLE = {
        'E', 'x', 'i', 'f', 0x00, 0x00, // ExifIdentifier (6bytes)
        'I', 'I',                       // TIFFByteOrder -> Little Endian check
        0x2a, 0x00,                     // TIFFIdentifier (2bytes)
        0x08, 0x00, 0x00, 0x00,         // IFD0thOffset (4bytes)
        0x11, 0x22, 0x33,
    };

    std::stringstream ss;
    JPEGReader reader(ss);

    stAPP1 app1{};

    EXPECT_FALSE(reader.getAPP1(mockDataShort.size(), app1, mockDataShort));    // Abnormal scenario
    EXPECT_FALSE(reader.getAPP1(mockDataNoExif.size(), app1, mockDataNoExif));  // No Exif

    // Little Endian
    EXPECT_TRUE(reader.getAPP1(mockDataLE.size(), app1, mockDataLE));
    EXPECT_EQ(app1.TIFFByteOrder[0], 'I');
    EXPECT_EQ(app1.TIFFByteOrder[1], 'I');
    EXPECT_EQ(reader.getEndian(), enEndian::littleEndian);

    // Big Endian
    EXPECT_TRUE(reader.getAPP1(mockDataBE.size(), app1, mockDataBE));
    EXPECT_EQ(app1.TIFFByteOrder[0], 'M');
    EXPECT_EQ(app1.TIFFByteOrder[1], 'M');
    EXPECT_EQ(reader.getEndian(), enEndian::bigEndian);
}


