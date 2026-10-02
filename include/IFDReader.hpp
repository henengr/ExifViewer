#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <gtest/gtest_prod.h>

#include "IFD.hpp"
#include "JPEGReader.hpp"

class IFDReader {
private:
    std::vector<std::uint8_t> &ardata;
    enEndian enendian;
    //
    FRIEND_TEST(IFDReaderTest, GetIFDValueByte);
    static bool getIFDValueByte(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> &data, stIFDTypeValue &value);
    FRIEND_TEST(IFDReaderTest, GetIFDValueAscii);
    static bool getIFDValueAscii(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> &data, stIFDTypeValue &value);
    FRIEND_TEST(IFDReaderTest, GetIFDValueShort);
    static bool getIFDValueShort(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> &data, stIFDTypeValue &value);
    FRIEND_TEST(IFDReaderTest, GetIFDValueLong);
    static bool getIFDValueLong(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> &data, stIFDTypeValue &value);
    FRIEND_TEST(IFDReaderTest, GetIFDValueRational);
    static bool getIFDValueRational(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> &data, stIFDTypeValue &value);
    FRIEND_TEST(IFDReaderTest, GetIFDValueUndefined);
    static bool getIFDValueUndefined(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> &data, stIFDTypeValue &value);
    FRIEND_TEST(IFDReaderTest, GetIFDValueSLong);
    static bool getIFDValueSLong(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> &data, stIFDTypeValue &value);
    FRIEND_TEST(IFDReaderTest, GetIFDValueSRational);
    static bool getIFDValueSRational(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> &data, stIFDTypeValue &value);

    FRIEND_TEST(IFDReaderTest, GetIFDValue);
    static bool getIFDValue(const std::uint32_t offset, const enEndian endian, const std::vector<std::uint8_t> &data,
                stIFDTypeValue &value);

protected:
public:
    //explicit IFDReader(){}
    IFDReader(std::vector<std::uint8_t> &data, const enEndian endian = enEndian::littleEndian):
        ardata(data), enendian(endian){}
    virtual ~IFDReader(){}

    bool getIFDValues(const std::uint32_t offset, std::vector<stIFDTypeValue> &values);
};

