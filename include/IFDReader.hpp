#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <variant>
#include <gtest/gtest_prod.h>

#include "IFD.hpp"
#include "JPEGReader.hpp"


struct stRational {
    std::uint32_t long1;
    std::uint32_t long2;
};

struct stSRational {
    std::int32_t slong1;
    std::int32_t slong2;
};

struct stIFD {
    std::uint16_t       tag;
    std::uint16_t       type;
    std::uint32_t       count;
    std::vector<std::uint8_t> data;
};

struct stIFDValue {
    stIFDTag            IFD;
    std::variant<
        std::vector<std::uint8_t>,      // BYTE
        std::string,                    // ASCII, UNDEFINED
        std::vector<std::uint16_t>,     // SHORT
        std::vector<std::uint32_t>,     // LONG
        std::vector<stRational>,        // RATIONAL
        std::vector<std::int32_t>,      // SLONG
        std::vector<stSRational>        // SRATIONAL
    > values;
};

constexpr std::int32_t IFDREADER_INVALID_IFD_SIZE                           = -1;
constexpr std::int32_t IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE    = -2;

constexpr const char* const IFD_UNKNOWN_TAG   = "???";

class IFDReader {
private:
    std::vector<std::uint8_t> &ardata;
    enEndian enendian;

    FRIEND_TEST(IFDReaderTest, readIFDByte);            // BYTE, ASCII, UNDEFINED
    FRIEND_TEST(IFDReaderTest, readIFDShort);           // SHORT
    FRIEND_TEST(IFDReaderTest, readIFDLong);            // LONG, SLONG
    FRIEND_TEST(IFDReaderTest, readIFDRational);        // RATIONAL, SRATIONAL
    static std::int32_t readIFD(const std::uint32_t offset, const enEndian endian, const std::vector<std::uint8_t> &data, stIFD &ifd);

    FRIEND_TEST(IFDReaderTest, getVariantIFDByte);      // BYTE, UNDEFINED
    FRIEND_TEST(IFDReaderTest, getVariantIFDAscii);     // ASCII
    FRIEND_TEST(IFDReaderTest, getVariantIFDShort);     // SHORT
    FRIEND_TEST(IFDReaderTest, getVariantIFDLong);      // LONG
    FRIEND_TEST(IFDReaderTest, getVariantIFDRational);  // RATIONAL
    FRIEND_TEST(IFDReaderTest, getVariantIFDSLong);     // SLONG
    FRIEND_TEST(IFDReaderTest, getVariantIFDSRational); // SRATIONAL
    static bool getVariantIFD(const stIFD &ifd, const enEndian endian, stIFDValue &value);
protected:
public:
    IFDReader(std::vector<std::uint8_t> &data, const enEndian endian = enEndian::littleEndian):
        ardata(data), enendian(endian){}
    virtual ~IFDReader(){}
};

