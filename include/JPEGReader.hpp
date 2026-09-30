#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>

class valueConverter {
public:
    static std::uint16_t swapUInt16(const std::uint16_t value);
    static std::uint32_t swapUInt32(const std::uint32_t value);
    static std::uint64_t swapUInt64(const std::uint64_t value);
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

class JPEGReader {
private:
    std::istream &stream;
    enEndian endian;
protected:
public:
    JPEGReader(std::istream &ifs):stream(ifs), endian(enEndian::littleEndian) {}
    virtual ~JPEGReader(){}
    enEndian getEndian() const;

    // Get current marker and doesn't seek the stream
    bool getMarker(std::uint16_t &marker);
    bool getLength(std::uint16_t &length);
    bool readSegment(const std::uint16_t length, std::vector<std::uint8_t> &data);

    bool getAPP0(const std::uint16_t length, stAPP0 &app0, const std::vector<std::uint8_t> data);
    bool getAPP1(const std::uint16_t length, stAPP1 &app1, const std::vector<std::uint8_t> data);
};

