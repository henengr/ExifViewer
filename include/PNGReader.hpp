#pragma once

#include <cstdint>
#include <fstream>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
#include "FileReader.hpp"


class PNGReader: public FileReader{
private:
    FRIEND_TEST(PNGReaderTest, readChunk);
    std::int32_t readChunk(const std::string fourCC, std::istream &stream, std::vector<std::uint8_t> &data);
protected:
public:
    PNGReader(std::istream &stream):FileReader(stream){} 
    PNGReader(const std::string path):FileReader(path){}
    virtual ~PNGReader(){}

    enEndian getEndian() const override;
    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

