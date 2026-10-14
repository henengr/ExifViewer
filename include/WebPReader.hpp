#pragma once

#include <cstdint>
#include <fstream>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
#include "FileReader.hpp"


class WebPReader: public FileReader{
private:
protected:
public:
    WebPReader(std::istream &stream):FileReader(stream){} 
    WebPReader(const std::string path):FileReader(path){}
    virtual ~WebPReader(){}

    enEndian getEndian() const override;
    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

