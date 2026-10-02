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
    stIFDTypeValue checkValue;
    stIFDTypeValue checkValueLong;
    stIFDTypeValue retValue;
    checkValue.type = enIFDType::BYTE;
    checkValue.byteValues = {0x56, 0x34, 0x12};
    checkValueLong.type = enIFDType::BYTE;
    checkValueLong.byteValues = {0x01, 0x23, 0x45, 0x67, 0x8a};

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::littleEndian, mockDataShort, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueByte(0x0500, 0, enEndian::littleEndian, mockDataShort, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::bigEndian, mockDataShort, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.byteValues, checkValue.byteValues);
    //
    EXPECT_TRUE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::littleEndian, mockDataLong, retValue));
    EXPECT_EQ(retValue.byteValues, checkValueLong.byteValues);
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
    stIFDTypeValue checkValue;
    stIFDTypeValue checkValueLong;
    stIFDTypeValue retValue;
    checkValue.type = enIFDType::ASCII;
    checkValue.asciiValue = "ABC\0";
    checkValueLong.type = enIFDType::ASCII;
    checkValueLong.asciiValue = "Exif_JPEG_422\0";

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::littleEndian, mockDataShort, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueAscii(0x0500, 0, enEndian::littleEndian, mockDataShort, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::bigEndian, mockDataShort, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.asciiValue, checkValue.asciiValue);
    //
    EXPECT_TRUE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::littleEndian, mockDataLong, retValue));
    EXPECT_EQ(retValue.asciiValue, checkValueLong.asciiValue);
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
    stIFDTypeValue checkValue;
    stIFDTypeValue checkValueLong;
    stIFDTypeValue retValue;
    checkValue.type = enIFDType::SHORT;
    checkValue.shortValues = {0x5678, 0x1234};
    checkValueLong.type = enIFDType::SHORT;
    checkValueLong.shortValues = {0x2301, 0x6745, 0xbc8a, 0x0000, 0x0000};

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::littleEndian, mockDataShort, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueShort(0x0500, 0, enEndian::littleEndian, mockDataShort, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::bigEndian, mockDataShort, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.shortValues, checkValue.shortValues);
    //
    EXPECT_TRUE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::littleEndian, mockDataLong, retValue));
    EXPECT_EQ(retValue.shortValues, checkValueLong.shortValues);
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
    stIFDTypeValue checkValue;
    stIFDTypeValue checkValueLong;
    stIFDTypeValue retValue;
    checkValue.type = enIFDType::LONG;
    checkValue.longValues = {0x12345678};
    checkValueLong.type = enIFDType::LONG;
    checkValueLong.longValues = {0x67452301, 0xbbaabc8a, 0x00000000, 0x00000000};

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::littleEndian, mockDataShort, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueLong(0x0500, 0, enEndian::littleEndian, mockDataShort, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::bigEndian, mockDataShort, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.longValues, checkValue.longValues);
    //
    EXPECT_TRUE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::littleEndian, mockDataLong, retValue));
    EXPECT_EQ(retValue.longValues, checkValueLong.longValues);
}

TEST(IFDReaderTest, GetIFDValue) {
    std::vector<std::uint8_t> mockDataByte = {
        0x05, 0x00,                     // Tag (0x0005: GPSAltitudeRef)
        0x01, 0x00,                     // Type (0x0001: BYTE)
        0x04, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x12, 0x34, 0x56, 0x78,
    };
    std::vector<std::uint8_t> mockDataAscii = {
        0x0e, 0x01,                     // Tag (0x010e: ImageDescription)
        0x02, 0x00,                     // Type (0x0002: ASCII)
        0x0e, 0x00, 0x00, 0x00,         // Count (0x0000000e)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        'E', 'x', 'i', 'f', '_',
        'J', 'P', 'E', 'G', '_', '4', '2', '2', 0x00,
    };
    std::vector<std::uint8_t> mockDataShort = {
        0x02, 0x01,                     // Tag (0x0102: BitsPerSample)
        0x03, 0x00,                     // Type (0x0003: SHORT)
        0x03, 0x00, 0x00, 0x00,         // Count (0x00000003)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x12, 0x34, 0x56, 0x78,
        0x11, 0x22, 0x33, 0x44,
    };
    std::vector<std::uint8_t> mockDataLong = {
        0x69, 0x87,                     // Tag (0x8769: Exif IFD Pointer)
        0x04, 0x00,                     // Type (0x0004: LONG)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x34, 0x12, 0x00, 0x00,
    };
    std::vector<std::uint8_t> checkValueByte = {0x12, 0x34, 0x56, 0x78};
    std::string checkValueAscii = "Exif_JPEG_422\0";
    std::vector<std::uint16_t> checkValueShort = {0x3412, 0x7856, 0x2211};
    std::vector<std::uint32_t> checkValueLong = {0x00001234};
    std::uint16_t checkTagByte = 0x0005;
    std::uint16_t checkTagAscii = 0x010e;
    std::uint16_t checkTagShort = 0x0102;
    std::uint16_t checkTagLong = 0x8769;

    IFDReader reader(mockDataAscii, enEndian::littleEndian);
    stIFDTypeValue value;
    bool result;

    // BYTE
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataByte, value);
    EXPECT_TRUE(result);
    EXPECT_EQ(value.byteValues, checkValueByte);
    EXPECT_EQ(value.tag.tag, checkTagByte);

    // ASCII
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataAscii, value);
    EXPECT_TRUE(result);
    EXPECT_EQ(value.asciiValue, checkValueAscii);
    EXPECT_EQ(value.tag.tag, checkTagAscii);

    // SHORT
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataShort, value);
    EXPECT_TRUE(result);
    EXPECT_EQ(value.shortValues, checkValueShort);
    EXPECT_EQ(value.tag.tag, checkTagShort);

    // LONG
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataLong, value);
    EXPECT_TRUE(result);
    EXPECT_EQ(value.longValues, checkValueLong);
    EXPECT_EQ(value.tag.tag, checkTagLong);
}

/*
TEST(IFDReaderTest, GetIFDValues) {
    std::vector<std::uint8_t> mockDataAscii = {
        'I', 'I',                       // Byte order (Little Endian)
        0x2a, 0x00,                     // 42
        0x08, 0x00, 0x00, 0x00,         // 0th IFD Offset
        0x01, 0x00,                     //
        //
        0x0e, 0x01,                     // Tag (0x010e: ImageDescription)
        0x02, 0x00,                     // Type (0x0002: ASCII)
        0x0e, 0x00, 0x00, 0x00,         // Count (0x0000000e)
        0x1a, 0x00, 0x00, 0x00,         // Offset (0x0000001a)
        0x00, 0x00, 0x00, 0x00,
        'E', 'x', 'i', 'f', '_',
        'J', 'P', 'E', 'G', '_', '4', '2', '2', 0x00,
    };
    std::string checkValueAscii = "Exif_JPEG_422\0";

    IFDReader reader(mockDataAscii, enEndian::littleEndian);
    std::vector<stIFDTypeValue> values;
    stIFDTypeValue value;

    bool result = reader.getIFDValues(0, values);
    value = values[0];

    EXPECT_TRUE(result);
    EXPECT_EQ(value.asciiValue, checkValueAscii);
}
*/