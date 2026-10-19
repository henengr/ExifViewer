#pragma once

#include <cstdint>
#include <fstream>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
#include "FileReader.hpp"


class HEICReader: public FileReader{
private:
    FRIEND_TEST(HEICReaderTest, readBoxes);
    std::int32_t readBoxes(std::string boxName, std::istream &stream, std::vector<std::vector<std::uint8_t>> &datas);
protected:
public:
    HEICReader(std::istream &stream):FileReader(stream){} 
    HEICReader(const std::string path):FileReader(path){}
    virtual ~HEICReader(){}

    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

