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

TEST(IFDReaderTest, GetIFDValueAscii) {
    std::vector<std::uint8_t> mockDataShort = {
        0x0e, 0x01,                     // Tag (0x010e: ImageDescription)
        0x02, 0x00,                     // Type (0x0002: ASCII)
        0x03, 0x00, 0x00, 0x00,         // Count (0x00000003)
    };
    std::vector<std::uint8_t> mockData = {
        0x0e, 0x01,                     // Tag (0x010e: ImageDescription)
        0x02, 0x00,                     // Type (0x0002: ASCII)
        0x04, 0x00, 0x00, 0x00,         // Count (0x00000004)
        'A', 'B', 'C', 0x00,            // Offset
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataLong = {
        0x0e, 0x01,                     // Tag (0x010e: ImageDescription)
        0x02, 0x00,                     // Type (0x0002: ASCII)
        0x0e, 0x00, 0x00, 0x00,         // Count (0x0000000e)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        'E', 'x', 'i', 'f', '_',
        'J', 'P', 'E', 'G', '_', '4', '2', '2', 0x00,
    };
    std::string checkValue = "ABC\0";
    std::string checkValueLong = "Exif_JPEG_422\0";
    std::string retValue;

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::littleEndian, mockDataShort, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueAscii(0x0500, 0, enEndian::littleEndian, mockDataShort, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::bigEndian, mockDataShort, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue, checkValue);
    //
    EXPECT_TRUE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::littleEndian, mockDataLong, retValue));
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

TEST(IFDReaderTest, GetIFDValueLong) {
    std::vector<std::uint8_t> mockDataShort = {
        0x16, 0x01,                     // Tag (0x0116: RowsPerStrip)
        0x04, 0x00,                     // Type (0x0004: LONG)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
    };
    std::vector<std::uint8_t> mockData = {
        0x16, 0x01,                     // Tag (0x0116: RowsPerStrip)
        0x04, 0x00,                     // Type (0x0004: LONG)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x78, 0x56, 0x34, 0x12,         // Offset (0x12345678)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataLong = {
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
    std::vector<std::uint32_t> checkValue = {0x12345678};
    std::vector<std::uint32_t> checkValueLong = {0x67452301, 0xbbaabc8a, 0x00000000, 0x00000000};
    std::vector<std::uint32_t> retValue;

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::littleEndian, mockDataShort, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueLong(0x0500, 0, enEndian::littleEndian, mockDataShort, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::bigEndian, mockDataShort, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue, checkValue);
    //
    EXPECT_TRUE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::littleEndian, mockDataLong, retValue));
    EXPECT_EQ(retValue, checkValueLong);
}

TEST(IFDReaderTest, GetIFDValue) {
    std::vector<std::uint8_t> mockDataAscii = {
        'I', 'I',                       // Byte order (Little Endian)
        0x2a, 0x00,                     // 42
        0x08, 0x00, 0x00, 0x00,         // 0th IFD Offset
        //
        0x0e, 0x01,                     // Tag (0x010e: ImageDescription)
        0x02, 0x00,                     // Type (0x0002: ASCII)
        0x0e, 0x00, 0x00, 0x00,         // Count (0x0000000e)
        0x18, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        'E', 'x', 'i', 'f', '_',
        'J', 'P', 'E', 'G', '_', '4', '2', '2', 0x00,
    };
    std::string checkValueAscii = "Exif_JPEG_422\0";

    IFDReader reader(mockDataAscii, enEndian::littleEndian);
    stIFDTypeValue value;

    bool result = reader.getIFDValue(0, value);

    EXPECT_TRUE(result);
    EXPECT_EQ(value.asciiValue, checkValueAscii);
}
