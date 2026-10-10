#include <string>
#include <vector>
#include <cstdint>

#include "HEICReader.hpp"


enEndian HEICReader::getEndian() const{
    return endian;
}

std::int32_t HEICReader::parse(std::vector<std::uint8_t> &data){
    std::int32_t ret = 0;
    this->endian = endian;

    return ret;
}


