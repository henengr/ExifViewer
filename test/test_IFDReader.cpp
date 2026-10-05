#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <variant>

#include "IFD.hpp"
#include "IFDReader.hpp"
#include "test_IFDReader.hpp"


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
        "",
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
        "",
        {'i', 'P', 'h', 'o', 'n', 'e', ' ', '1', '8', ' ', 'P', 'r', 'o', 0x00}
    };
    std::string checkVal = "iPhone 18 Pro";

    stIFDValue retVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    EXPECT_EQ(std::get<std::string>(retVal.values), checkVal);
}

TEST(IFDReaderTest, getVariantIFDShort){
    stIFD ifd = {
        0x0112,     // Tag (0x0112: Orientation)
        0x0003,     // Type (0x0003: SHORT)
        0x00000005,     // Count (0x00000005)
        "",
        {0x01, 0x23, 0x45, 0x67, 0x8a, 0xbc, 0x00, 0x00, 0x00, 0x00}
    };
    std::vector<std::uint16_t> checkVal = {0x2301, 0x6745, 0xbc8a, 0x0000, 0x0000};

    stIFDValue retVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    EXPECT_EQ(std::get<std::vector<std::uint16_t>>(retVal.values), checkVal);
}

TEST(IFDReaderTest, getVariantIFDLong){
    stIFD ifd = {
        0x0116,     // Tag (0x0112: RowsPerStrip)
        0x0004,     // Type (0x0004: LONG)
        0x00000004,     // Count (0x00000004)
        "",
        {0x01, 0x23, 0x45, 0x67, 0x8a, 0xbc, 0xaa, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };
    std::vector<std::uint32_t> checkVal = {0x67452301, 0xbbaabc8a, 0x00000000, 0x00000000};

    stIFDValue retVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    EXPECT_EQ(std::get<std::vector<std::uint32_t>>(retVal.values), checkVal);
}

TEST(IFDReaderTest, getVariantIFDRational){
    stIFD ifd = {
        0x011a,     // Tag (0x011a: XResolution)
        0x0005,     // Type (0x0005: RATIONAL)
        0x00000001,     // Count (0x00000001)
        "",
        {0x48, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00}
    };
    stRational checkVal = {0x00000048, 0x0000010};

    stIFDValue retVal;
    stRational ratVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    ratVal = std::get<std::vector<stRational>>(retVal.values)[0];
    EXPECT_EQ(ratVal.long1, checkVal.long1);
    EXPECT_EQ(ratVal.long2, checkVal.long2);
}

TEST(IFDReaderTest, getVariantIFDSLong){
    stIFD ifd = {
        0xffa0,     // Tag (0xffa0: -)
        0x0009,     // Type (0x0009: SLONG)
        0x00000004,     // Count (0x00000004)
        "",
        {0x01, 0x23, 0x45, 0x67, 0x8a, 0xbc, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
    };
    std::vector<std::int32_t> checkVal = {0x67452301, 0x00aabc8a, 0x00000000, 0x00000000};

    stIFDValue retVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    EXPECT_EQ(std::get<std::vector<std::int32_t>>(retVal.values), checkVal);
}

TEST(IFDReaderTest, getVariantIFDSRational){
    stIFD ifd = {
        0x9203,     // Tag (0x9203: BrightnessValue)
        0x000a,     // Type (0x000a: SRATIONAL)
        0x00000001,     // Count (0x00000001)
        "",
        {0x48, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00}
    };
    stSRational checkVal = {0x00000048, 0x0000010};

    stIFDValue retVal;
    stSRational ratVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    ratVal = std::get<std::vector<stSRational>>(retVal.values)[0];
    EXPECT_EQ(ratVal.slong1, checkVal.slong1);
    EXPECT_EQ(ratVal.slong2, checkVal.slong2);
}

TEST(IFDReaderTest, getVariantIFDUTF8){
    stIFD ifd = {
        0x0110,     // Tag (0x0110: Model)
        0x0081,     // Type (0x0081: UTF-8)
        0x00000020,     // Count (0x00000020)
        "",
        {0xe6, 0x98, 0x8e, 0xe6, 0x99, 0xba, 0xe3, 0x81, 0x82, 0xe3, 0x82, 0x93, 0xe3, 0x81, 0xaa, 0x26,    // "明智あんな&"
         0xe5, 0xb0, 0x8f, 0xe6, 0x9e, 0x97, 0xe3, 0x81, 0xbf, 0xe3, 0x81, 0x8f, 0xe3, 0x82, 0x8b, 0x00}    // "小林みくる\0"
    };
    std::string checkVal = "明智あんな&小林みくる";

    stIFDValue retVal;
    EXPECT_TRUE(IFDReader::getVariantIFD(ifd, enEndian::littleEndian, retVal));
    EXPECT_EQ(std::get<std::string>(retVal.values), checkVal);
}

TEST(IFDReaderTest, readIFDs){
    std::vector<stIFD> ifds;
    std::int32_t ret = IFDReader::readIFDs(0, APP1_INNER_DATA, ifds);
    //EXPECT_EQ(ifds.size(), 12);

    stIFDValue ifd;
    IFDReader::getVariantIFD(ifds[0], enEndian::bigEndian, ifd);        // Make
    EXPECT_EQ(0x010f, ifd.IFD.tag);
    EXPECT_EQ("Apple", std::get<std::string>(ifd.values));
    IFDReader::getVariantIFD(ifds[1], enEndian::bigEndian, ifd);        // Model
    EXPECT_EQ(0x0110, ifd.IFD.tag);
    EXPECT_EQ("iPhone 16 Pro", std::get<std::string>(ifd.values));
    IFDReader::getVariantIFD(ifds[2], enEndian::bigEndian, ifd);        // Orientation
    EXPECT_EQ(0x0112, ifd.IFD.tag);
    EXPECT_TRUE(std::get<std::vector<std::uint16_t>>(ifd.values).size() > 0);
    EXPECT_EQ(0x0001, std::get<std::vector<std::uint16_t>>(ifd.values)[0]);
    IFDReader::getVariantIFD(ifds[3], enEndian::bigEndian, ifd);        // XResolution
    EXPECT_EQ(0x011a, ifd.IFD.tag);
    EXPECT_TRUE(std::get<std::vector<stRational>>(ifd.values).size() > 0);
    EXPECT_EQ(0x00000048, std::get<std::vector<stRational>>(ifd.values)[0].long1);
    EXPECT_EQ(0x00000001, std::get<std::vector<stRational>>(ifd.values)[0].long2);
    IFDReader::getVariantIFD(ifds[4], enEndian::bigEndian, ifd);        // YResolution
    EXPECT_EQ(0x011b, ifd.IFD.tag);
    EXPECT_TRUE(std::get<std::vector<stRational>>(ifd.values).size() > 0);
    EXPECT_EQ(0x00000048, std::get<std::vector<stRational>>(ifd.values)[0].long1);
    EXPECT_EQ(0x00000001, std::get<std::vector<stRational>>(ifd.values)[0].long2);
    IFDReader::getVariantIFD(ifds[5], enEndian::bigEndian, ifd);        // ResolutionUnit
    EXPECT_EQ(0x0128, ifd.IFD.tag);
    EXPECT_TRUE(std::get<std::vector<std::uint16_t>>(ifd.values).size() > 0);
    EXPECT_EQ(0x0002, std::get<std::vector<std::uint16_t>>(ifd.values)[0]);
    IFDReader::getVariantIFD(ifds[6], enEndian::bigEndian, ifd);        // Software
    EXPECT_EQ(0x0131, ifd.IFD.tag);
    EXPECT_EQ("26.6.1", std::get<std::string>(ifd.values));
    IFDReader::getVariantIFD(ifds[7], enEndian::bigEndian, ifd);        // DateTime
    EXPECT_EQ(0x0132, ifd.IFD.tag);
    EXPECT_EQ("2026:09:05 11:54:52", std::get<std::string>(ifd.values));    
    IFDReader::getVariantIFD(ifds[8], enEndian::bigEndian, ifd);        // ???
    EXPECT_EQ(0x013c, ifd.IFD.tag);
    EXPECT_EQ("iPhone 16 Pro", std::get<std::string>(ifd.values));
    IFDReader::getVariantIFD(ifds[9], enEndian::bigEndian, ifd);        // YCbCrPositioning
    EXPECT_EQ(0x0213, ifd.IFD.tag);
    EXPECT_TRUE(std::get<std::vector<std::uint16_t>>(ifd.values).size() > 0);
    EXPECT_EQ(0x0001, std::get<std::vector<std::uint16_t>>(ifd.values)[0]);
    IFDReader::getVariantIFD(ifds[10], enEndian::bigEndian, ifd);        // Exif IFD Pointer
    EXPECT_EQ(0x8769, ifd.IFD.tag);
    EXPECT_TRUE(std::get<std::vector<std::uint32_t>>(ifd.values).size() > 0);
    EXPECT_EQ(0x000000ec, std::get<std::vector<std::uint32_t>>(ifd.values)[0]);
    IFDReader::getVariantIFD(ifds[11], enEndian::bigEndian, ifd);        // GPS Info IFD Pointer
    EXPECT_EQ(0x8825, ifd.IFD.tag);
    EXPECT_TRUE(std::get<std::vector<std::uint32_t>>(ifd.values).size() > 0);
    EXPECT_EQ(0x00000ab4, std::get<std::vector<std::uint32_t>>(ifd.values)[0]);
    //
}
