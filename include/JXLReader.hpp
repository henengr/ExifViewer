#pragma once

#include <cstdint>
#include <fstream>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
#include "FileReader.hpp"


// JPEG XL File reader class
class JXLReader: public FileReader{
private:
    FRIEND_TEST(JXLReaderTest, readBox);
    std::int32_t readBox(std::string boxName, std::istream &stream, std::vector<std::uint8_t> &data);
protected:
public:
    JXLReader(std::istream &stream):FileReader(stream){} 
    JXLReader(const std::string path):FileReader(path){}
    virtual ~JXLReader(){}

    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

