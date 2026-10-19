#pragma once

#include <cstdint>
#include <fstream>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
#include "FileReader.hpp"


class TIFFReader: public FileReader{
private:
protected:
public:
    TIFFReader(std::istream &stream):FileReader(stream){} 
    TIFFReader(const std::string path):FileReader(path){}
    virtual ~TIFFReader(){}

    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

