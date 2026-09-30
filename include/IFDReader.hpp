#pragma once

#include <cstdint>
#include <vector>
#include <gtest/gtest_prod.h>

#include "IFD.hpp"
#include "JPEGReader.hpp"

class IFDReader {
private:
    std::vector<std::uint8_t> ardata;
    enEndian enendian;
    //
    FRIEND_TEST(IFDReaderTest, GetIFDValueByte);
    FRIEND_TEST(IFDReaderTest, GetIFDValueShort);
    FRIEND_TEST(IFDReaderTest, GetIFDValueLong);
    static bool getIFDValueByte(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> data, std::vector<std::uint8_t> &value);
    static bool getIFDValueShort(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
                const std::vector<std::uint8_t> data, std::vector<std::uint16_t> &value);

protected:
public:
    explicit IFDReader(){}
    IFDReader(std::vector<std::uint8_t> &data, const enEndian endian = enEndian::littleEndian):
        ardata(data), enendian(endian){}
    virtual ~IFDReader(){}

    bool getIFDValue(const std::uint16_t tag, const std::uint32_t offset, stIFDTypeValue &value);
};

