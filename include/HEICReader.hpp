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
protected:
public:
    HEICReader(std::istream &stream):FileReader(stream){} 
    HEICReader(const std::string path):FileReader(path){}
    virtual ~HEICReader(){}

    enEndian getEndian() const override;
    std::int32_t getImageInfo(stImageInfo &info) override;
    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

