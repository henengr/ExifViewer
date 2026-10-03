#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <variant>

#include "IFD.hpp"
#include "IFDReader.hpp"



TEST(IFDReaderTest, readIFDByte){
    std::vector<std::uint8_t> mockDataErr = {
        0x05, 0x00,                     // Tag (0x0005: GPSAltitudeRef)
        0x01, 0x00,                     // Type (0x0001: BYTE)
        0x03, 0x00, 0x00, 0x00,         // Count (0x00000003)
    };
    std::vector<std::uint8_t> mockDataErr2 = {
        0x05, 0x00,                     // Tag (0x0005: GPSAltitudeRef)
        0x01, 0x00,                     // Type (0x0001: BYTE)
        0x05, 0x00, 0x00, 0x00,         // Count (0x00000005)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData = {
        0x05, 0x00,                     // Tag (0x0005: GPSAltitudeRef)
        0x01, 0x00,                     // Type (0x0001: BYTE)
        0x03, 0x00, 0x00, 0x00,         // Count (0x00000003)
        0x56, 0x34, 0x12, 0x00,         // Offset (0x00123456)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData2 = {
        0x05, 0x00,                     // Tag (0x0005: GPSAltitudeRef)
        0x01, 0x00,                     // Type (0x0001: BYTE)
        0x05, 0x00, 0x00, 0x00,         // Count (0x00000005)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x01, 0x23, 0x45, 0x67, 0x8a,   // data (0x01, 0x23, 0x45, 0x67, 0x8a)
    };

    stIFD ifd;
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockDataErr, ifd), IFDREADER_INVALID_IFD_SIZE);
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockDataErr2, ifd), IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE);
    std::vector<std::uint8_t> retVal = {0x56, 0x34, 0x12};
    std::vector<std::uint8_t> retVal2 = {0x01, 0x23, 0x45, 0x67, 0x8a};
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockData, ifd), 12);
    EXPECT_EQ(ifd.tag, 0x0005);
    EXPECT_EQ(ifd.type, 0x0001);
    EXPECT_EQ(ifd.count, 0x0003);
    EXPECT_EQ(ifd.data, retVal);
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockData2, ifd), 12);
    EXPECT_EQ(ifd.tag, 0x0005);
    EXPECT_EQ(ifd.type, 0x0001);
    EXPECT_EQ(ifd.count, 0x0005);
    EXPECT_EQ(ifd.data, retVal2);
}

TEST(IFDReaderTest, readIFDShort){
    std::vector<std::uint8_t> mockDataErr = {
        0x12, 0x01,                     // Tag (0x0112: Orientation)
        0x03, 0x00,                     // Type (0x0003: SHORT)
        0x02, 0x00, 0x00, 0x00,         // Count (0x00000002)
    };
    std::vector<std::uint8_t> mockDataErr2 = {
        0x12, 0x01,                     // Tag (0x0112: Orientation)
        0x03, 0x00,                     // Type (0x0003: SHORT)
        0x05, 0x00, 0x00, 0x00,         // Count (0x00000005)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData = {
        0x12, 0x01,                     // Tag (0x0112: Orientation)
        0x03, 0x00,                     // Type (0x0003: SHORT)
        0x02, 0x00, 0x00, 0x00,         // Count (0x00000002)
        0x78, 0x56, 0x34, 0x12,         // Offset (0x12345678)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData2 = {
        0x12, 0x01,                     // Tag (0x0112: Orientation)
        0x03, 0x00,                     // Type (0x0003: SHORT)
        0x05, 0x00, 0x00, 0x00,         // Count (0x00000005)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x01, 0x23,                     // data (0x01, 0x23)
        0x45, 0x67,                     // data (0x45, 0x67)
        0x8a, 0xbc,                     // data (0x8a, 0xbc)
        0x00, 0x00,
        0x00, 0x00,
    };

    stIFD ifd;
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockDataErr, ifd), IFDREADER_INVALID_IFD_SIZE);
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockDataErr2, ifd), IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE);
    std::vector<std::uint8_t> retVal = {0x78, 0x56, 0x34, 0x12};
    std::vector<std::uint8_t> retVal2 = {0x01, 0x23, 0x45, 0x67, 0x8a, 0xbc, 0x00, 0x00, 0x00, 0x00};
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockData, ifd), 12);
    EXPECT_EQ(ifd.tag, 0x0112);
    EXPECT_EQ(ifd.type, 0x0003);
    EXPECT_EQ(ifd.count, 0x0002);
    EXPECT_EQ(ifd.data, retVal);
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockData2, ifd), 12);
    EXPECT_EQ(ifd.tag, 0x0112);
    EXPECT_EQ(ifd.type, 0x0003);
    EXPECT_EQ(ifd.count, 0x0005);
    EXPECT_EQ(ifd.data, retVal2);
}

TEST(IFDReaderTest, readIFDLong){
    std::vector<std::uint8_t> mockDataErr = {
        0x16, 0x01,                     // Tag (0x0116: RowsPerStrip)
        0x04, 0x00,                     // Type (0x0004: LONG)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
    };
    std::vector<std::uint8_t> mockDataErr2 = {
        0x16, 0x01,                     // Tag (0x0116: RowsPerStrip)
        0x04, 0x00,                     // Type (0x0004: LONG)
        0x04, 0x00, 0x00, 0x00,         // Count (0x00000004)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData = {
        0x16, 0x01,                     // Tag (0x0116: RowsPerStrip)
        0x04, 0x00,                     // Type (0x0004: LONG)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x78, 0x56, 0x34, 0x12,         // Offset (0x12345678)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData2 = {
        0x16, 0x01,                     // Tag (0x0116: RowsPerStrip)
        0x04, 0x00,                     // Type (0x0004: LONG)
        0x04, 0x00, 0x00, 0x00,         // Count (0x00000004)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x01, 0x23, 0x45, 0x67,         // data (0x01, 0x23, 0x45, 0x67)
        0x8a, 0xbc, 0xaa, 0xbb,         // data (0x8a, 0xbc, 0xaa, 0xbb)
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };

    stIFD ifd;
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockDataErr, ifd), IFDREADER_INVALID_IFD_SIZE);
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockDataErr2, ifd), IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE);
    std::vector<std::uint8_t> retVal = {0x78, 0x56, 0x34, 0x12};
    std::vector<std::uint8_t> retVal2 = {0x01, 0x23, 0x45, 0x67, 0x8a, 0xbc, 0xaa, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockData, ifd), 12);
    EXPECT_EQ(ifd.tag, 0x0116);
    EXPECT_EQ(ifd.type, 0x0004);
    EXPECT_EQ(ifd.count, 0x0001);
    EXPECT_EQ(ifd.data, retVal);
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockData2, ifd), 12);
    EXPECT_EQ(ifd.tag, 0x0116);
    EXPECT_EQ(ifd.type, 0x0004);
    EXPECT_EQ(ifd.count, 0x0004);
    EXPECT_EQ(ifd.data, retVal2);
}

TEST(IFDReaderTest, readIFDRational){
    std::vector<std::uint8_t> mockDataErr = {
        0x1a, 0x01,                     // Tag (0x011a: XResolution)
        0x05, 0x00,                     // Type (0x0005: RATIONAL)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
    };
    std::vector<std::uint8_t> mockDataErr2 = {
        0x1a, 0x01,                     // Tag (0x011a: XResolution)
        0x05, 0x00,                     // Type (0x0005: RATIONAL)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData = {
        0x1a, 0x01,                     // Tag (0x011a: XResolution)
        0x05, 0x00,                     // Type (0x0005: RATIONAL)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x48, 0x00, 0x00, 0x00,
        0x10, 0x00, 0x00, 0x00,
    };

    stIFD ifd;
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockDataErr, ifd), IFDREADER_INVALID_IFD_SIZE);
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockDataErr2, ifd), IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE);
    std::vector<std::uint8_t> retVal = {0x48, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00};
    EXPECT_EQ(IFDReader::readIFD(0, enEndian::littleEndian, mockData, ifd), 12);
    EXPECT_EQ(ifd.tag, 0x011a);
    EXPECT_EQ(ifd.type, 0x0005);
    EXPECT_EQ(ifd.count, 0x0001);
    EXPECT_EQ(ifd.data, retVal);
}

TEST(IFDReaderTest, getVariantIFDByte){
    stIFD ifd = {
        0x0005,     // Tag (0x0005: GPSAltitudeRef)
        0x0001,     // Type (0x0001: BYTE)
        0x00000005,     // Count (0x00000005)
        {0x01, 0x23, 0x45, 0x67, 0x8a}
    };
    std::vector<std::uint8_t> checkVal = {0x01, 0x23, 0x45, 0x67, 0x8a};

    stIFDValue retVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    EXPECT_EQ(std::get<std::vector<std::uint8_t>>(retVal.values), checkVal);
}

TEST(IFDReaderTest, getVariantIFDAscii){
    stIFD ifd = {
        0x0110,     // Tag (0x0110: Model)
        0x0002,     // Type (0x0002: ASCII)
        0x0000000d,     // Count (0x0000000d)
        {'i', 'P', 'h', 'o', 'n', 'e', ' ', '1', '8', 'P', 'r', 'o', 0x00}
    };
    std::string checkVal = "iPhone 18Pro";

    stIFDValue retVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    EXPECT_EQ(std::get<std::string>(retVal.values), checkVal);
}

TEST(IFDReaderTest, getVariantIFDShort){
    stIFD ifd = {
        0x0112,     // Tag (0x0112: Orientation)
        0x0003,     // Type (0x0003: SHORT)
        0x00000005,     // Count (0x00000005)
        {0x01, 0x23, 0x45, 0x67, 0x8a, 0xbc, 0x00, 0x00, 0x00, 0x00}
    };
    std::vector<uint16_t> checkVal = {0x2301, 0x6745, 0xbc8a, 0x0000, 0x0000};

    stIFDValue retVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    EXPECT_EQ(std::get<std::vector<std::uint16_t>>(retVal.values), checkVal);
}
