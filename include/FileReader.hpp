#pragma once

#include <vector>

#include "JFIF.hpp"
#include "IFD.hpp"

enum class enEndian: std::uint8_t {
    littleEndian = 1,
    bigEndian
};

class FileReader {
protected:
    std::istream &arstream;
    std::vector<std::uint8_t> ardata;
    enEndian endian;
public:
    FileReader(std::ifstream &stream): arstream(stream), endian(enEndian::littleEndian), ardata(){} 
};

