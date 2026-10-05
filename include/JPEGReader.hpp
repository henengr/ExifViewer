#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <gtest/gtest_prod.h>

#include "FileReader.hpp"
#include "JFIF.hpp"


constexpr std::int32_t JPEGREADER_INVALID_FILE_SIZE                 = -1;

class valueConverter {
public:
    static std::uint16_t swapUInt16(const std::uint16_t value);
    static std::uint32_t swapUInt32(const std::uint32_t value);
    static std::uint64_t swapUInt64(const std::uint64_t value);
    //
    static std::uint8_t streamToUInt8(std::istream &ifs);
    static std::uint16_t streamToUInt16(std::istream &ifs);
    static std::uint32_t streamToUInt32(std::istream &ifs);
    static std::uint64_t streamToUInt64(std::istream &ifs);
    static void dispDumb(std::vector<uint8_t> &data);
};

enum class enEndian: std::uint8_t {
    littleEndian = 1,
    bigEndian
};

class JPEGReader: public FileReader{
private:

    FRIEND_TEST(JPEGReaderTest, readSegment);
    static std::int32_t readSegment(std::uint16_t marker, std::istream &stream, std::vector<std::uint8_t> &data);

protected:
public:
    explicit JPEGReader(){}
    virtual ~JPEGReader(){}
};

