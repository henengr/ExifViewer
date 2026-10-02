#include <gtest/gtest.h>
#include <vector>
#include <cstdint>

#include "IFD.hpp"
#include "IFDReader.hpp"


TEST(IFDReaderTest, GetIFDValueByte) {
    std::vector<std::uint8_t> mockDataErr = {
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
    std::vector<std::uint8_t> mockData2 = {
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
    EXPECT_FALSE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::littleEndian, mockDataErr, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueByte(0x0500, 0, enEndian::littleEndian, mockDataErr, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::bigEndian, mockDataErr, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.byteValues, checkValue.byteValues);
    //
    EXPECT_TRUE(IFDReader::getIFDValueByte(0x0005, 0, enEndian::littleEndian, mockData2, retValue));
    EXPECT_EQ(retValue.byteValues, checkValueLong.byteValues);
}

TEST(IFDReaderTest, GetIFDValueAscii) {
    std::vector<std::uint8_t> mockDataErr = {
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
    std::vector<std::uint8_t> mockData2 = {
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
    EXPECT_FALSE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::littleEndian, mockDataErr, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueAscii(0x0500, 0, enEndian::littleEndian, mockDataErr, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::bigEndian, mockDataErr, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.asciiValue, checkValue.asciiValue);
    //
    EXPECT_TRUE(IFDReader::getIFDValueAscii(0x010e, 0, enEndian::littleEndian, mockData2, retValue));
    EXPECT_EQ(retValue.asciiValue, checkValueLong.asciiValue);
}

TEST(IFDReaderTest, GetIFDValueShort) {
    std::vector<std::uint8_t> mockDataErr = {
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
    stIFDTypeValue checkValue;
    stIFDTypeValue checkValueLong;
    stIFDTypeValue retValue;
    checkValue.type = enIFDType::SHORT;
    checkValue.shortValues = {0x5678, 0x1234};
    checkValueLong.type = enIFDType::SHORT;
    checkValueLong.shortValues = {0x2301, 0x6745, 0xbc8a, 0x0000, 0x0000};

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::littleEndian, mockDataErr, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueShort(0x0500, 0, enEndian::littleEndian, mockDataErr, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::bigEndian, mockDataErr, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.shortValues, checkValue.shortValues);
    //
    EXPECT_TRUE(IFDReader::getIFDValueShort(0x0112, 0, enEndian::littleEndian, mockData2, retValue));
    EXPECT_EQ(retValue.shortValues, checkValueLong.shortValues);
}

TEST(IFDReaderTest, GetIFDValueLong) {
    std::vector<std::uint8_t> mockDataErr = {
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
    stIFDTypeValue checkValue;
    stIFDTypeValue checkValueLong;
    stIFDTypeValue retValue;
    checkValue.type = enIFDType::LONG;
    checkValue.longValues = {0x12345678};
    checkValueLong.type = enIFDType::LONG;
    checkValueLong.longValues = {0x67452301, 0xbbaabc8a, 0x00000000, 0x00000000};

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::littleEndian, mockDataErr, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueLong(0x0500, 0, enEndian::littleEndian, mockDataErr, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::bigEndian, mockDataErr, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.longValues, checkValue.longValues);
    //
    EXPECT_TRUE(IFDReader::getIFDValueLong(0x0116, 0, enEndian::littleEndian, mockData2, retValue));
    EXPECT_EQ(retValue.longValues, checkValueLong.longValues);
}

TEST(IFDReaderTest, GetIFDValueRational) {
    std::vector<std::uint8_t> mockDataErr = {
        0x1a, 0x01,                     // Tag (0x011a: XResolution)
        0x05, 0x00,                     // Type (0x0005: RATIONAL)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
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
    stRational checkValue;
    stIFDTypeValue retValue;
    checkValue = {0x00000048, 0x00000010};

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueRational(0x011a, 0, enEndian::littleEndian, mockDataErr, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueRational(0x0500, 0, enEndian::littleEndian, mockDataErr, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueRational(0x011a, 0, enEndian::bigEndian, mockDataErr, retValue));      // invalid endian setting

    // Normal Scenario
    stRational ratValue;
    EXPECT_TRUE(IFDReader::getIFDValueRational(0x011a, 0, enEndian::littleEndian, mockData, retValue));
    ratValue = retValue.rationalValues[0];
    EXPECT_EQ(ratValue.long1, checkValue.long1);
    EXPECT_EQ(ratValue.long2, checkValue.long2);
}

TEST(IFDReaderTest, GetIFDValueUndefined) {
    std::vector<std::uint8_t> mockDataErr = {
        0x00, 0x90,                     // Tag (0x9000: ExifVersion)
        0x07, 0x00,                     // Type (0x0007: UNDEFINED)
        0x04, 0x00, 0x00, 0x00,         // Count (0x00000004)
    };
    std::vector<std::uint8_t> mockData = {
        0x00, 0x90,                     // Tag (0x9000: ExifVersion)
        0x07, 0x00,                     // Type (0x0007: UNDEFINED)
        0x04, 0x00, 0x00, 0x00,         // Count (0x00000004)
        0x30, 0x32, 0x30, 0x30,         // Offset (0x30303230)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData2 = {  // Maybe it will not actually be used.
        0x00, 0x90,                     // Tag (0x9000: ExifVersion)
        0x07, 0x00,                     // Type (0x0007: UNDEFINED)
        0x06, 0x00, 0x00, 0x00,         // Count (0x00000006)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x30, 0x32, 0x30, 0x30,
        0x38, 0x39, 0x00, 0x00,
    };
    stIFDTypeValue checkValue;
    stIFDTypeValue checkValueLong;
    stIFDTypeValue retValue;
    checkValue.type = enIFDType::UNDEFINED;
    checkValue.undefinedValue = {0x30, 0x32, 0x30, 0x30};
    checkValueLong.type = enIFDType::UNDEFINED;
    checkValueLong.undefinedValue = {0x30, 0x32, 0x30, 0x30, 0x38, 0x39};

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueUndefined(0x9000, 0, enEndian::littleEndian, mockDataErr, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueUndefined(0x0500, 0, enEndian::littleEndian, mockDataErr, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueUndefined(0x9000, 0, enEndian::bigEndian, mockDataErr, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueUndefined(0x9000, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.undefinedValue, checkValue.undefinedValue);
    //
    EXPECT_TRUE(IFDReader::getIFDValueUndefined(0x9000, 0, enEndian::littleEndian, mockData2, retValue));
    EXPECT_EQ(retValue.undefinedValue, checkValueLong.undefinedValue);
}

TEST(IFDReaderTest, GetIFDValueSLong) {
    std::vector<std::uint8_t> mockDataErr = {
        0xa0, 0xff,                     // Tag (0xffa0: -)
        0x09, 0x00,                     // Type (0x0009: SLONG)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
    };
    std::vector<std::uint8_t> mockData = {
        0xa0, 0xff,                     // Tag (0xffa0: -)
        0x09, 0x00,                     // Type (0x0009: SLONG)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x78, 0x56, 0x34, 0x12,         // Offset (0x12345678)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockData2 = {
        0xa0, 0xff,                     // Tag (0xffa0: -)
        0x09, 0x00,                     // Type (0x0009: SLONG)
        0x06, 0x00, 0x00, 0x00,         // Count (0x00000006)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x01, 0x23, 0x45, 0x67,         // data (0x01, 0x23, 0x45, 0x67)
        0x8a, 0xbc, 0xaa, 0x00,         // data (0x8a, 0xbc, 0xaa, 0x00)
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };
    stIFDTypeValue checkValue;
    stIFDTypeValue checkValueSLong;
    stIFDTypeValue retValue;
    checkValue.type = enIFDType::SLONG;
    checkValue.slongValues = {0x12345678};
    checkValueSLong.type = enIFDType::LONG;
    checkValueSLong.slongValues = {0x67452301, 0x00aabc8a, 0x00000000, 0x00000000};

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueSLong(0xffa0, 0, enEndian::littleEndian, mockDataErr, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueSLong(0x0500, 0, enEndian::littleEndian, mockDataErr, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueSLong(0xffa0, 0, enEndian::bigEndian, mockDataErr, retValue));      // invalid endian setting

    // Normal Scenario
    EXPECT_TRUE(IFDReader::getIFDValueSLong(0xffa0, 0, enEndian::littleEndian, mockData, retValue));
    EXPECT_EQ(retValue.slongValues, checkValue.slongValues);
    //
  //  EXPECT_TRUE(IFDReader::getIFDValueSLong(0xffa0, 0, enEndian::littleEndian, mockData2, retValue));
  //  EXPECT_EQ(retValue.slongValues, checkValueSLong.slongValues);
}

TEST(IFDReaderTest, GetIFDValueSRational) {
    std::vector<std::uint8_t> mockDataErr = {
        0x03, 0x92,                     // Tag (0x9203: BrightnessValue)
        0x0a, 0x00,                     // Type (0x000A: SRATIONAL)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
    };
    std::vector<std::uint8_t> mockData = {
        0x03, 0x92,                     // Tag (0x9203: BrightnessValue)
        0x0a, 0x00,                     // Type (0x000A: SRATIONAL)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x48, 0x00, 0x00, 0x00,
        0x10, 0x00, 0x00, 0x00,
    };
    stSRational checkValue;
    stIFDTypeValue retValue;
    checkValue = {0x00000048, 0x00000010};

    // Abnormal Scenario
    EXPECT_FALSE(IFDReader::getIFDValueSRational(0x9203, 0, enEndian::littleEndian, mockDataErr, retValue));   // short data
    EXPECT_FALSE(IFDReader::getIFDValueSRational(0x0500, 0, enEndian::littleEndian, mockDataErr, retValue));   // invalid tag
    EXPECT_FALSE(IFDReader::getIFDValueSRational(0x9203, 0, enEndian::bigEndian, mockDataErr, retValue));      // invalid endian setting

    // Normal Scenario
    stSRational ratValue;
    EXPECT_TRUE(IFDReader::getIFDValueSRational(0x9203, 0, enEndian::littleEndian, mockData, retValue));
    ratValue = retValue.srationalValues[0];
    EXPECT_EQ(ratValue.slong1, checkValue.slong1);
    EXPECT_EQ(ratValue.slong2, checkValue.slong2);
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
    std::vector<std::uint8_t> mockDataRational = {
        0x1a, 0x01,                     // Tag (0x011a: XResolution)
        0x05, 0x00,                     // Type (0x0005: RATIONAL)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x48, 0x00, 0x00, 0x00,
        0x10, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataUndefined = {
        0x00, 0x90,                     // Tag (0x9000: ExifVersion)
        0x07, 0x00,                     // Type (0x0007: UNDEFINED)
        0x04, 0x00, 0x00, 0x00,         // Count (0x00000004)
        0x30, 0x32, 0x30, 0x30,         // Offset (0x30303230)
        0x00, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataSLong = {
        0x99, 0x99,                     // Tag (0x9999: ***DummySLongForTest***)
        0x09, 0x00,                     // Type (0x0009: SLONG)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x34, 0x12, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataSRational = {
        0x03, 0x92,                     // Tag (0x9203: BrightnessValue)
        0x0a, 0x00,                     // Type (0x000A: SRATIONAL)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x10, 0x00, 0x00, 0x00,         // Offset (0x00000010)
        0x00, 0x00, 0x00, 0x00,
        0x48, 0x00, 0x00, 0x00,
        0x10, 0x00, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataInvaidType = {
        0x69, 0x87,                     // Tag (0x8769: Exif IFD Pointer)
        0x3f, 0x00,                     // Type (0x003f: -)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x34, 0x12, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataNonExistantTag = {
        0xff, 0xff,                     // Tag (0xffff: -)
        0x04, 0x00,                     // Type (0x0004: LONG)
        0x01, 0x00, 0x00, 0x00,         // Count (0x00000001)
        0x34, 0x12, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataInvalidCount = {
        0x69, 0x87,                     // Tag (0x8769: Exif IFD Pointer)
        0x04, 0x00,                     // Type (0x0004: LONG)
        0x00, 0x00, 0x00, 0x00,         // Count (0x00000000)
        0x34, 0x12, 0x00, 0x00,
    };
    std::vector<std::uint8_t> mockDataInvalidOffset = {
        0x02, 0x01,                     // Tag (0x0102: BitsPerSample)
        0x03, 0x00,                     // Type (0x0003: SHORT)
        0x03, 0x00, 0x00, 0x00,         // Count (0x00000003)
        0x3f, 0x00, 0x00, 0x00,         // Offset (0x0000003f)
    };
    //
    std::vector<std::uint8_t> checkValueByte = {0x12, 0x34, 0x56, 0x78};
    std::string checkValueAscii = "Exif_JPEG_422\0";
    std::vector<std::uint16_t> checkValueShort = {0x3412, 0x7856, 0x2211};
    std::vector<std::uint32_t> checkValueLong = {0x00001234};
    stRational checkValueRational = {0x00000048, 0x00000010};
    std::vector<std::uint8_t> checkValueUndefined = {0x30, 0x32, 0x30, 0x30};
    std::vector<std::int32_t> checkValueSLong = {0x00001234};
    stSRational checkValueSRational = {0x00000048, 0x00000010};
    //
    std::uint16_t checkTagByte = 0x0005;
    std::uint16_t checkTagAscii = 0x010e;
    std::uint16_t checkTagShort = 0x0102;
    std::uint16_t checkTagLong = 0x8769;
    std::uint16_t checkTagRational = 0x011a;
    std::uint16_t checkTagUndefined = 0x9000;
    std::uint16_t checkTagSLong = 0x9999;
    std::uint16_t checkTagSRational = 0x9203;

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

    // RATIONAL
    stRational ratValue;
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataRational, value);
    EXPECT_TRUE(result);
    ratValue = value.rationalValues[0];
    EXPECT_EQ(ratValue.long1, checkValueRational.long1);
    EXPECT_EQ(ratValue.long2, checkValueRational.long2);
    EXPECT_EQ(value.tag.tag, checkTagRational);

    // UNDEFINED
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataUndefined, value);
    EXPECT_TRUE(result);
    EXPECT_EQ(value.undefinedValue, checkValueUndefined);
    EXPECT_EQ(value.tag.tag, checkTagUndefined);

    // SLONG
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataSLong, value);
    EXPECT_TRUE(result);
    EXPECT_EQ(value.slongValues, checkValueSLong);
    EXPECT_EQ(value.tag.tag, checkTagSLong);

    // SRATIONAL
    stSRational sratValue;
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataSRational, value);
    EXPECT_TRUE(result);
    sratValue = value.srationalValues[0];
    EXPECT_EQ(sratValue.slong1, checkValueSRational.slong1);
    EXPECT_EQ(sratValue.slong2, checkValueSRational.slong2);
    EXPECT_EQ(value.tag.tag, checkTagSRational);

    // Invalid Type value
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataInvaidType, value);
    EXPECT_FALSE(result);

    // Non-Existant Tag value
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataNonExistantTag, value);
    EXPECT_FALSE(result);

    // Invalid Count value
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataInvalidCount, value);
    EXPECT_FALSE(result);

    // Invalid Offset value
    result = reader.getIFDValue(0, enEndian::littleEndian, mockDataInvalidOffset, value);
    EXPECT_FALSE(result);
}

TEST(IFDReaderTest, GetIFDValues) {
    std::vector<std::uint8_t> mockData = {
        // +0x00000000
        'M', 'M',
        0x00, 0x2A,
        0x00, 0x00, 0x00, 0x08,
        0x00, 0x0C,
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
        0x87, 0x69, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xEC,     // 0x8769: Exif IFD Pointer
        0x88, 0x25, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0A, 0xB4,     // 0x8825: GPS Info IFD Pointer
        0x00, 0x00, 0x0B, 0xEA,                                                     // Next IFD Offset
        // +0x0000009e
        'A', 'p', 'p', 'l', 'e', 0x00,                                              // Make
        'i', 'P', 'h', 'o', 'n', 'e', ' ', '1', '6', 'P', 'r', 'o', 0x00,           // Model
        0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x01,                             // XResolution
        0x00, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 0x01,                             // YResolution
        '2', '6', '.', '6', '.', '1', 0x00, 0x00,                                   // ResolutionUnit
        '2', '0', '2', '6', ':', '0', '9', ':', '0', '5', ' ', '1', '1', ':', '5', '4', ':', '5', '2', 0x00,    // DateTime
        'i', 'P', 'h', 'o', 'n', 'e', ' ', '1', '6', ' ', 'P', 'r', 'o', 0x00,      // ???
    };
    std::string checkValueAscii = "Apple\0";

    IFDReader reader(mockData, enEndian::bigEndian);
    std::vector<stIFDTypeValue> values;
    stIFDTypeValue value;

    bool result = reader.getIFDValues(0, values);

    EXPECT_TRUE(result);

    value = values[0];  // 
    EXPECT_EQ(value.asciiValue, checkValueAscii);
}

