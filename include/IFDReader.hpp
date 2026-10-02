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

    FRIEND_TEST(IFDReaderTest, readIFDByte);
    FRIEND_TEST(IFDReaderTest, readIFDShort);
    FRIEND_TEST(IFDReaderTest, readIFDLong);
    FRIEND_TEST(IFDReaderTest, readIFDRational);
    static std::int32_t readIFD(const std::uint32_t offset, const enEndian endian, const std::vector<std::uint8_t> &data,
                stIFD &ifd);
protected:
public:
    IFDReader(std::vector<std::uint8_t> &data, const enEndian endian = enEndian::littleEndian):
        ardata(data), enendian(endian){}
    virtual ~IFDReader(){}
};

