#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iostream>

#include "WebPReader.hpp" 


TEST(WebPReaderTest, readHeader){
    std::vector<std::uint8_t> mock = {
        'R', 'I', 'F', 'F',         // fourCC
        0x00, 0x01, 0x02, 0x03,     // fileSize
        'W', 'E', 'B', 'P',         //
        'V', 'P', '8', 'X',         //
        0x0a, 0x00, 0x00, 0x00,     // 
        0b00001100,                 // flags
        0x00, 0x00, 0x00,           // reserved
        0xff, 0x03, 0x00,           // canvasWidth
        0xff, 0x02, 0x00,           // canvasHeight
    };

    auto is = mock.begin();
    auto ie = mock.end();
    std::string str(is, ie);
    std::stringstream ss(str);
    stWebPVP8XHeader header;

    auto pwebp = std::make_unique<WebPReader>("");
    std::int32_t ret = pwebp->readHeader(ss, header);
    EXPECT_EQ(ret, 30);
    EXPECT_EQ(header.fourCC, "RIFF");
    EXPECT_EQ(header.fileSize, 0x03020100);
    EXPECT_EQ(header.webP, "WEBP");
    EXPECT_EQ(header.chunkHeader, "VP8X");
    EXPECT_EQ(header.flagAnimation, false);
    EXPECT_EQ(header.flagXMP, true);
    EXPECT_EQ(header.flagExif, true);
    EXPECT_EQ(header.flagAlpha, false);
    EXPECT_EQ(header.flagICCProfile, false);
    EXPECT_EQ(header.canvasWidth, 0x00000400);
    EXPECT_EQ(header.canvasHeight, 0x00000300);
}

TEST(WebPReaderTest, readChunk){
    std::vector<std::uint8_t> mock = {
        'T', 'S', 'T', '1',
        0x07, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x00,
        'T', 'S', 'T', '2',
        0x0c, 0x00, 0x00, 0x00,
        0xaa, 0xbb, 0xcc, 0xdd, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
    };
    std::vector<std::uint8_t> checkVal = {0xaa, 0xbb, 0xcc, 0xdd, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

    auto is = mock.begin();
    auto ie = mock.end();
    std::string str(is, ie);
    std::stringstream ss(str);
    stWebPVP8XHeader header;
    std::vector<std::uint8_t> data;

    auto pwebp = std::make_unique<WebPReader>("");
    std::int32_t ret = pwebp->readChunk("TST2", ss, data);
    EXPECT_EQ(ret, 20);
    EXPECT_EQ(checkVal, data);
}


TEST(WebPReaderTest, parse){
    std::vector<std::uint8_t> mock = {
        'R', 'I', 'F', 'F',         // fourCC
        0x00, 0x01, 0x02, 0x03,     // fileSize
        'W', 'E', 'B', 'P',         //
        'V', 'P', '8', 'X',         //
        0x0a, 0x00, 0x00, 0x00,     // 
        0b00001100,                 // flags
        0x00, 0x00, 0x00,           // reserved
        0x00, 0x04, 0x00,           // canvasWidth
        0x00, 0x03, 0x00,           // canvasHeight
        'E', 'X', 'I', 'F',
        0xEC, 0x00, 0x00, 0x00,
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

    std::vector<std::uint8_t> data;
    std::vector<std::uint8_t> checkVal(mock.begin() + 38, mock.end());
    std::string_view view(reinterpret_cast<const char*>(mock.data()), mock.size());
    std::istringstream strm((std::string(view)));
    auto pwebp = std::make_unique<WebPReader>(strm);
    std::int32_t ret = pwebp->parse(data);
    EXPECT_TRUE(ret > 0);
    //EXPECT_EQ(checkVal.size(), data.size());
    EXPECT_EQ(checkVal, data);
}

