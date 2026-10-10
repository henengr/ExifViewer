#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
#include "FileReader.hpp"
#include "JFIF.hpp"


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

class JPEGReader: public FileReader{
private:
    stAPP0 app0;
    stAPP1 app1;

    FRIEND_TEST(JPEGReaderTest, readSegment);
    FRIEND_TEST(JPEGReaderTest, readSegmentErr);
    std::int32_t readSegment(std::uint16_t marker, std::istream &stream, std::vector<std::uint8_t> &data);

    FRIEND_TEST(JPEGReaderTest, parseAPP0);
    FRIEND_TEST(JPEGReaderTest, parseAPP0Err);
    std::int32_t parseAPP0(const std::vector<std::uint8_t>& data, stAPP0 &value);

    FRIEND_TEST(JPEGReaderTest, parseAPP1);
    FRIEND_TEST(JPEGReaderTest, parseAPP1Err);
    std::int32_t parseAPP1(const std::vector<std::uint8_t>& data, enEndian &endian, stAPP1 &value);
protected:
public:
    JPEGReader(std::ifstream &stream):FileReader(stream){} 
    JPEGReader(const std::string path):FileReader(path){}
    virtual ~JPEGReader(){}

    enEndian getEndian() const override;
    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

