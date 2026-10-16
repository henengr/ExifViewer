#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iostream>

#include "PNGReader.hpp" 


TEST(PNGReaderTest, readChunk){
    std::vector<std::uint8_t> mock = {
        0x00, 0x00, 0x00, 0x04,
        'T', 'S', 'T', '1',
        0x00, 0x01, 0x02, 0x03,
        0xaa, 0xbb, 0xcc, 0xdd,
        //
        0x00, 0x00, 0x00, 0x04,
        'T', 'S', 'T', '2',
        0x07, 0x06, 0x05, 0x04,
        0x11, 0x22, 0x33, 0x44,
    };

    auto is = mock.begin();
    auto ie = mock.end();
    std::string str(is, ie);
    std::stringstream ss(str);
    std::vector<std::uint8_t> data;
    std::vector<std::uint8_t> checkVal = {0x07, 0x06, 0x05, 0x04};

    auto ppng = std::make_unique<PNGReader>("");
    std::int32_t ret = ppng->readChunk("TST2", ss, data);
    EXPECT_EQ(ret, 16);
    EXPECT_EQ(checkVal, data);
}

TEST(PNGReaderTest, getIHDRInfo){
    std::vector<std::uint8_t> mock = {
        // IHDR chunk
        0x00, 0x00, 0x00, 0x0D,                                 // payload size
        'I', 'H', 'D', 'R',
        0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00,
        0x08, 0x02, 0x00, 0x00, 0x00, 
        0x6D, 0xFA, 0x3F, 0x3A,                                 // CRC
    };

    auto is = mock.begin();
    auto ie = mock.end();
    std::string str(is, ie);
    std::stringstream ss(str);
    stPNG_IHDRData ihdr;

    auto ppng = std::make_unique<PNGReader>("");
    std::int32_t ret = ppng->getIHDRInfo(ss, ihdr);
    EXPECT_EQ(ret, 25);
    EXPECT_EQ(ihdr.width, 0x800);
    EXPECT_EQ(ihdr.height, 0x600);
    EXPECT_EQ(ihdr.bitDepth, 8);
    EXPECT_EQ(ihdr.colorType, enPNG_ColorType::RGBTriple);
    EXPECT_EQ(ihdr.compressionMethod, enPNG_CompressionMethod::none);
    EXPECT_EQ(ihdr.filterMethod, enPNG_FilterMethod::none);
    EXPECT_EQ(ihdr.interlaceMethod, enPNG_InterlaceMethod::leftToRight_TopToBottom);
}

TEST(PNGReaderTest, getImageInfo){
    std::vector<std::uint8_t> mock = {
        0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A,            // PNG file signature
        // IHDR chunk
        0x00, 0x00, 0x00, 0x0D,                                 // payload size
        'I', 'H', 'D', 'R',
        0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00,
        0x08, 0x02, 0x00, 0x00, 0x00, 
        0x6D, 0xFA, 0x3F, 0x3A,                                 // CRC
    };

    auto is = mock.begin();
    auto ie = mock.end();
    std::string str(is, ie);
    std::stringstream ss(str);
    stImageInfo info;

    auto ppng = std::make_unique<PNGReader>(ss);
    std::int32_t ret = ppng->getImageInfo(info);
    EXPECT_EQ(ret, 25);
    EXPECT_EQ(info.width, 0x800);
    EXPECT_EQ(info.height, 0x600);
    EXPECT_EQ(info.bitsPerPixel, 8);
}

TEST(PNGReaderTest, parse){
    std::vector<std::uint8_t> mock = {
        0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A,            // PNG file signature
        // IHDR chunk
        0x00, 0x00, 0x00, 0x0D,                                 // payload size
        'I', 'H', 'D', 'R',
        0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00,
        0x08, 0x02, 0x00, 0x00, 0x00, 
        0x6D, 0xFA, 0x3F, 0x3A,                                 // CRC
        // eXIf chunk
        0x00, 0x00, 0x00, 0xec,                                 // payload size
        'e', 'X', 'I', 'f',
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
        //
        0x00, 0x00, 0x00, 0x00,                                 // CRC
    };

    std::vector<std::uint8_t> data;
    std::vector<std::uint8_t> checkVal(mock.begin() + 41, mock.end() - 4);  // Subtract the CRC portion
    std::string_view view(reinterpret_cast<const char*>(mock.data()), mock.size());
    std::istringstream strm((std::string(view)));
    auto ppng = std::make_unique<PNGReader>(strm);
    std::int32_t ret = ppng->parse(data);
    EXPECT_TRUE(ret > 0);
    //EXPECT_EQ(checkVal.size(), data.size());
    EXPECT_EQ(checkVal, data);
}

