#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class valueConverter {
public:
    static std::uint16_t swapUInt16(const std::uint16_t value);
    static std::uint32_t swapUInt32(const std::uint32_t value);
    static std::uint64_t swapUInt64(const std::uint64_t value);
    static std::uint8_t streamToUInt8(std::ifstream &ifs);
    static std::uint16_t streamToUInt16(std::ifstream &ifs);
    static std::uint32_t streamToUInt32(std::ifstream &ifs);
    static std::uint64_t streamToUInt64(std::ifstream &ifs);
};

class JPEGReader {
private:
protected:
public:
    explicit JPEGReader();
    virtual ~JPEGReader();
};

