#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <variant>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
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
    std::uint32_t       offset;
    std::string         description;
    std::vector<std::uint8_t> data;
};

using variantIFD = std::variant<
    std::vector<std::uint8_t>,      // BYTE, UNDEFINED
    std::string,                    // ASCII
    std::vector<std::uint16_t>,     // SHORT
    std::vector<std::uint32_t>,     // LONG
    std::vector<stRational>,        // RATIONAL
    std::vector<std::int32_t>,      // SLONG
    std::vector<stSRational>        // SRATIONAL
>;
struct stIFDValue {
    stIFDTag            IFD;
    variantIFD          values;
};

constexpr std::int32_t IFDREADER_INVALID_IFD_SIZE                           = -1;
constexpr std::int32_t IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE    = -2;

constexpr std::int32_t IFDREADER_INVALID_ENDIAN_SETTING                     = -8;
constexpr std::int32_t IFDREADER_INVALID_HEADER42_VALUE                     = -9;
constexpr std::int32_t IFDREADER_INVALID_NUM_OF_IFDS                        = -10;


constexpr const char* const IFD_UNKNOWN_TAG   = "???";


class IFDReader {
private:
    std::vector<std::uint8_t> &ardata;
    enEndian enendian;

    FRIEND_TEST(IFDReaderTest, readIFDByte);            // BYTE, ASCII, UNDEFINED, UTF-8
    FRIEND_TEST(IFDReaderTest, readIFDShort);           // SHORT
    FRIEND_TEST(IFDReaderTest, readIFDLong);            // LONG, SLONG
    FRIEND_TEST(IFDReaderTest, readIFDRational);        // RATIONAL, SRATIONAL
    static std::int32_t readIFD(const std::uint32_t offset, const enEndian endian, const std::vector<std::uint8_t> &data, stIFD &ifd);

    FRIEND_TEST(IFDReaderTest, readIFDs);
    static std::int32_t readIFDs(const std::uint32_t offset, const std::vector<std::uint8_t> &data, std::vector<stIFD> &ifds);
protected:
public:
    IFDReader(std::vector<std::uint8_t> &data, const enEndian endian = enEndian::littleEndian):
        ardata(data), enendian(endian){}
    virtual ~IFDReader(){}
    static bool getVariantIFD(const stIFD &ifd, const enEndian endian, stIFDValue &value);
    std::int32_t readIFDs(std::vector<stIFD> &ifds);
};

