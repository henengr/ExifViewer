#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>

class valueConverter {
public:
    static std::uint16_t swapUInt16(const std::uint16_t value);
    static std::uint32_t swapUInt32(const std::uint32_t value);
    static std::uint64_t swapUInt64(const std::uint64_t value);
    static std::uint8_t streamToUInt8(std::ifstream &ifs);
    static std::uint16_t streamToUInt16(std::ifstream &ifs);
    static std::uint32_t streamToUInt32(std::ifstream &ifs);
    static std::uint64_t streamToUInt64(std::ifstream &ifs);
    static void dispDumb(std::vector<char> &data);
};

class JPEGReader {
private:
    std::ifstream &stream;
protected:
public:
    JPEGReader(std::ifstream &ifs):stream(ifs){}
    virtual ~JPEGReader(){}

    // Get current marker and doesn't seek the stream
    bool getMarker(std::uint16_t &marker);
    bool getLength(std::uint16_t &length);
    bool readSegment(const std::uint16_t length, std::vector<char> &data);

    bool readAPP0(stAPP0 &app0);
};

