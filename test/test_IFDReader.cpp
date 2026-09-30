#include <gtest/gtest.h>
#include <vector>
#include <cstdint>

#include "IFD.hpp"
#include "IFDReader.hpp"


TEST(IFDReaderTest, GetIFDValueByte) {
    std::vector<std::uint8_t> mockDataShort = {
        0x05, 0x00,                     // Tag (0x0005: GPSAltitudeRef)
        0x01, 0x00,                     // Type (0x0001: BYTE)
        0x03, 0x00, 0x00, 0x00,         // Count (0x00000003)
    };
    std::vector<std::uint8_t> mockData = {
        0x05, 0x00,                     // Tag (0x0005: GPSAltitudeRef)
        0x01, 0x00,                     // Type (0x0001: BYTE)
        0x03, 0x00, 0x00, 0x00,         // Count (0x00000003)
        0x56, 0x34, 0x12, 0x00,         // Offset (0x00123456)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataLong = {
        0x05, 0x00,                     // Tag (0x0005: GPSAltitudeRef)
        0x01, 0x00,                     // Type (0x0001: BYTE)
        0x05, 0x00, 0x00, 0x00,         // Count (0x00000005)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x01, 0x23, 0x45, 0x67, 0x8a,   // data (0x01, 0x23, 0x45, 0x67, 0x8a)
    };
    std::vector<std::uint8_t> checkValue = {0x56, 0x34, 0x12};
    std::vector<std::uint8_t> checkValueLong = {0x01, 0x23, 0x45, 0x67, 0x8a};
    std::vector<std::uint8_t> retValue;

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::littleEndian, mockDataShort, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueByte(0x0500, 0, enEndian::littleEndian, mockDataShort, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::bigEndian, mockDataShort, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue, checkValue);
    //
    EXPECT_TRUE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::littleEndian, mockDataLong, retValue));
    EXPECT_EQ(retValue, checkValueLong);
}

TEST(IFDReaderTest, GetIFDValueShort) {
    std::vector<std::uint8_t> mockDataShort = {
        0x12, 0x01,                     // Tag (0x0112: Orientation)
        0x03, 0x00,                     // Type (0x0003: SHORT)
        0x02, 0x00, 0x00, 0x00,         // Count (0x00000002)
    };
    std::vector<std::uint8_t> mockData = {
        0x12, 0x01,                     // Tag (0x0112: Orientation)
        0x03, 0x00,                     // Type (0x0003: SHORT)
        0x02, 0x00, 0x00, 0x00,         // Count (0x00000002)
        0x78, 0x56, 0x34, 0x12,         // Offset (0x12345678)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataLong = {
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
    std::vector<std::uint16_t> checkValue = {0x5678, 0x1234};
    std::vector<std::uint16_t> checkValueLong = {0x2301, 0x6745, 0xbc8a, 0x0000, 0x0000};
    std::vector<std::uint16_t> retValue;

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::littleEndian, mockDataShort, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueShort(0x0500, 0, enEndian::littleEndian, mockDataShort, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::bigEndian, mockDataShort, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue, checkValue);
    //
    EXPECT_TRUE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::littleEndian, mockDataLong, retValue));
    EXPECT_EQ(retValue, checkValueLong);
}

