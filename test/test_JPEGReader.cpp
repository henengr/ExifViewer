#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string_view>

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
        0x01, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x2c, 0x00, 0x00, 0x41, 0x4d, 0x50, 0x46,
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
        0x01, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x2c, 0x00, 0x00, 0x41, 0x4d, 0x50, 0x46};
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

    std:int32_t len;
    auto pjpg = std::make_unique<JPEGReader>("");
    len = pjpg->readSegment(static_cast<std::uint16_t>(enSegmentMarker::APP0), ss, data);
    EXPECT_EQ(dtAPP0, data);

    data.clear();
    len = pjpg->readSegment(static_cast<std::uint16_t>(enSegmentMarker::APP1), ss, data);
    EXPECT_EQ(dtAPP1, data);
}

TEST(JPEGReaderTest, readSegmentErr){
    std::vector<std::uint8_t> mockErr1 = {
        0xff, 0xd8,             // SOI
    };
    std::vector<std::uint8_t> mockErr2 = {
        0xff, 0xd8,             // SOI
        0xff, 0xe0,             // APP0
        0x00, 0x14,
        0x4a, 0x46, 0x49, 0x46, 0x00,       // "JFIF\0"
    };
    std::vector<std::uint8_t> mockErr3 = {
        0xff, 0xd8,             // SOI
        0xff, 0xe0,             // APP0
        0x00, 0x14,
        0x4a, 0x46, 0x49, 0x46, 0x00,       // "JFIF\0"
        0x01, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x2c, 0x00, 0x00, 0x41, 0x4d, 0x50, 
    };

    auto is1 = mockErr1.begin();
    auto ie1 = mockErr1.end();
    std::string str1(is1, ie1);
    std::stringstream ss1(str1);
    auto is2 = mockErr2.begin();
    auto ie2 = mockErr2.end();
    std::string str2(is2, ie2);
    std::stringstream ss2(str2);
    std::vector<std::uint8_t> data;

    std:int32_t len;
    auto pjpg = std::make_unique<JPEGReader>("");
    len = pjpg->readSegment(static_cast<std::uint16_t>(enSegmentMarker::APP0), ss1, data);
    EXPECT_EQ(len, FILEREADER_INVALID_FILE_FORMAT);

    len = pjpg->readSegment(static_cast<std::uint16_t>(enSegmentMarker::APP0), ss2, data);
    EXPECT_EQ(len, FILEREADER_INVALID_FILE_FORMAT);
}

TEST(JPEGReaderTest, parseAPP0){
    std::vector<std::uint8_t> mock = {
        0x4a, 0x46, 0x49, 0x46, 0x00,       // "JFIF\0"
        0x01, 0x01,                         // version
        0x01,                               // pixelConcentrationUnit
        0x01, 0x2c,                         // pixelConcentrationX
        0x01, 0x2c,                         // pixelConcentrationY
        0x00,               // thumbnailImageWidth
        0x00,               // thumbnailImageHeight
        0x41, 0x4d, 0x50, 0x46      // thumbnailImage
    };
    std::vector<std::uint8_t> dtAPP0 = {0x4a, 0x46, 0x49, 0x46, 0x00,       // "JFIF\0"
        0x01, 0x01, 0x01, 0x01, 0x2c, 0x01, 0x2c, 0x00, 0x00, 0x41, 0x4d, 0x50, 0x46};
    char dtJFIF[] = {0x4a, 0x46, 0x49, 0x46, 0x00};
    std::vector<std::uint8_t> dtThumb = {0x41, 0x4d, 0x50, 0x46};

    stAPP0 app;
    auto pjpg = std::make_unique<JPEGReader>("");
    std::int32_t len = pjpg->parseAPP0(mock, app);
    EXPECT_EQ(std::memcmp(app.JFIFIdentifier, dtJFIF, 5), 0);
    EXPECT_EQ(app.version, 0x0101);
    EXPECT_EQ(app.pixelConcentrationUnit, 0x01);
    EXPECT_EQ(app.pixelConcentrationX, 0x012c);
    EXPECT_EQ(app.thumbnailImageWidth, 0x00);
    EXPECT_EQ(app.thumbnailImageHeight, 0x00);
    EXPECT_EQ(app.thumbnailImage, dtThumb);
}

TEST(JPEGReaderTest, parseAPP0Err){
    std::vector<std::uint8_t> mockErr1 = {
        0x4a, 0x46, 0x49
    };
    std::vector<std::uint8_t> mockErr2 = {
        0x4a, 0x46, 0x49, 0x47, 0x00,       // "JFIG\0"
        0x01, 0x01,                         // version
        0x01,                               // pixelConcentrationUnit
        0x01, 0x2c,                         // pixelConcentrationX
        0x01, 0x2c,                         // pixelConcentrationY
        0x00,               // thumbnailImageWidth
        0x00,               // thumbnailImageHeight
        0x41, 0x4d, 0x50, 0x46      // thumbnailImage
    };

    stAPP0 app;
    std::int32_t len;
    auto pjpg = std::make_unique<JPEGReader>("");
    len = pjpg->parseAPP0(mockErr1, app);
    EXPECT_EQ(len, FILEREADER_INVALID_FILE_SIZE);

    len = pjpg->parseAPP0(mockErr2, app);
    EXPECT_EQ(len, FILEREADER_INVALID_FILE_FORMAT);
}

TEST(JPEGReaderTest, parseAPP1){
    std::vector<std::uint8_t> mock = {
        0x45, 0x78, 0x69, 0x66, 0x00, 0x00, // "Exif\0\0"
        0x4d, 0x4d,             // "MM"
        0x00, 0x2a,
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00,
    };
    char dtExif[] = {0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
    std::vector<std::uint8_t> dtThumb = {0x41, 0x4d, 0x50, 0x46};

    stAPP1 app;
    enEndian endian;
    auto pjpg = std::make_unique<JPEGReader>("");
    std::int32_t len = pjpg->parseAPP1(mock, endian, app);
    EXPECT_EQ(std::memcmp(app.ExifIdentifier, dtExif, 6), 0);
    EXPECT_EQ(endian, enEndian::bigEndian);
}

TEST(JPEGReaderTest, parseAPP1Err){
    std::vector<std::uint8_t> mockErr1 = {
        0x45, 0x78, 0x69,
    };
    std::vector<std::uint8_t> mockErr2 = {
        0x45, 0x78, 0x69, 0x67, 0x00, 0x00, // "Exig\0\0"
        0x4d, 0x4d,             // "MM"
        0x00, 0x2a,
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x00,
    };
    char dtExif[] = {0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
    std::vector<std::uint8_t> dtThumb = {0x41, 0x4d, 0x50, 0x46};

    stAPP1 app;
    enEndian endian;
    std::int32_t len;
    auto pjpg = std::make_unique<JPEGReader>("");
    len = pjpg->parseAPP1(mockErr1, endian, app);
    EXPECT_EQ(len, FILEREADER_INVALID_FILE_SIZE);

    len = pjpg->parseAPP1(mockErr2, endian, app);
    EXPECT_EQ(len, FILEREADER_INVALID_FILE_FORMAT);
    EXPECT_TRUE(std::memcmp(app.ExifIdentifier, dtExif, 6) > 0);
}

TEST(JPEGReaderTest, parse){
    std::vector<std::uint8_t> mock = {
        0xFF, 0xD8,
        0xFF, 0xE0, 0x00, 0x14, 0x4A, 0x46, 0x49, 0x46, 0x00,
        0x01, 0x01, 0x01, 0x01, 0x2C, 0x01, 0x2C, 0x00, 0x00, 0x41, 0x4D, 0x50, 0x46,
        // APP1
        0xFF, 0xE1,
        0x00, 0xf4,     // Segment length
        //
        0x45, 0x78, 0x69, 0x66, 0x00, 0x00, // "Exif\0\0"
        // +0x00000000
        'M', 'M',                   // Byte Order (Big Endian)
        0x00, 0x2A,                 // 42
        0x00, 0x00, 0x00, 0x08,     // 0th IFD Offset
        0x00, 0x0C,                 // Number of Entries
        // +0x0000000a
        0x01, 0x0F, 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x9E,     // 0x010f: Make
        0x01, 0x10, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0xA4,     // 0x0110: Model
        0x01, 0x12, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,     // 0x0112: Orientation
        0x01, 0x1A, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xB2,     // 0x011a: XResolution
        0x01, 0x1B, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xBA,     // 0x011b: YResolution
        0x01, 0x28, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00,     // 0x0128: ResolutionUnit
        0x01, 0x31, 0x00, 0x02, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0xC2,     // 0x0131: Software
        0x01, 0x32, 0x00, 0x02, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xCA,     // 0x0132: DateTime
        0x01, 0x3C, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0xDE,     // 0x013c: ???
        0x02, 0x13, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00,     // 0x0213: YCbCrPositioning
        0x82, 0x9a, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0xa2,     // 0x829a: ExposureTime
        0x82, 0x9d ,0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0xaa,     // 0x829d: FNumber
        0x00, 0x00, 0x00, 0x00,                                                     // Next IFD Offset
        // +0x0000009e
        'A', 'p', 'p', 'l', 'e', 0x00,                                              // Make
        'i', 'P', 'h', 'o', 'n', 'e', ' ', '1', '6', ' ', 'P', 'r', 'o', 0x00,      // Model
        0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x01,                             // XResolution
        0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x01,                             // YResolution
        '2', '6', '.', '6', '.', '1', 0x00, 0x00,                                   // Software
        '2', '0', '2', '6', ':', '0', '9', ':', '0', '5', ' ', '1', '1', ':', '5', '4', ':', '5', '2', 0x00,    // DateTime
        'i', 'P', 'h', 'o', 'n', 'e', ' ', '1', '6', ' ', 'P', 'r', 'o', 0x00,      // ???
    };

    stAPP1 app;
    enEndian endian;
    std::vector<std::uint8_t> data;
    std::vector<std::uint8_t> checkVal(mock.begin() + 34, mock.end());
    std::string_view view(reinterpret_cast<const char*>(mock.data()), mock.size());
    std::istringstream strm((std::string(view)));
    auto pjpg = std::make_unique<JPEGReader>(strm);
    std::int32_t ret = pjpg->parse(data);
    EXPECT_TRUE(ret > 0);
    //EXPECT_EQ(checkVal.size(), data.size());
    EXPECT_EQ(checkVal, data);
}
