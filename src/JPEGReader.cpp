#include <string>
#include <vector>
#include <iomanip>
#include <cctype>

#include "JFIF.hpp"
#include "JPEGReader.hpp"

std::uint16_t valueConverter::swapUInt16(const std::uint16_t value){
    uint16_t value1 = value >> 8;
    uint16_t value2 = value << 8;
    return value1 | value2;
}

std::uint32_t valueConverter::swapUInt32(const std::uint32_t value){
    std::uint32_t value1 = swapUInt16(value >> 16);
    std::uint32_t value2 = static_cast<std::uint32_t>(swapUInt16(value & 0xffff)) << 16;
    return value1 | value2;
}

std::uint64_t valueConverter::swapUInt64(const std::uint64_t value){
    std::uint64_t value1 = swapUInt32(value >> 32);
    std::uint64_t value2 = static_cast<std::uint64_t>(swapUInt32(value & 0xffffffff)) << 32;
    return value1 | value2;
}

std::uint8_t valueConverter::streamToUInt8(std::ifstream &ifs){
    uint8_t ret = 0;
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(ret));
    return ret;
}

std::uint16_t valueConverter::streamToUInt16(std::ifstream &ifs){
    uint16_t ret = 0;
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(ret));
    return ret;
}

std::uint32_t valueConverter::streamToUInt32(std::ifstream &ifs){
    uint32_t ret = 0;
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(ret));
    return ret;
}

std::uint64_t valueConverter::streamToUInt64(std::ifstream &ifs){
    uint64_t ret = 0LL;
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(ret));
    return ret;
}

void valueConverter::dispDumb(std::vector<char> &data){
    std::ios::fmtflags f(std::cout.flags());
    std::size_t count = 0;
    std::string ascii = "";
    for (std::vector<char>::iterator i = data.begin(); i != data.end(); ++i){
        std::uint8_t byte = static_cast<std::uint8_t>(*i);
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
        if (std::isprint(byte)){
            ascii += static_cast<char>(byte);
        }else{
            ascii += '.';
        }
        count++;
        if (count % 16 == 0){
            std::cout << " | " << ascii << std::endl;
            ascii = "";
        }
    }
    if (count % 16){
        std::size_t missingSpaces = 16 - (count % 16);
        for (std::size_t j = 0; j < missingSpaces; j++){
            std::cout << "   ";
        }
        std::cout << " | " << ascii << std::endl;
    }
    std::cout.flags(f);
}


bool JPEGReader::getMarker(std::uint16_t &marker){
    stream.read(reinterpret_cast<char*>(&marker), sizeof(marker));
    marker = valueConverter::swapUInt16(marker);
    return true;
}

bool JPEGReader::getLength(std::uint16_t &length){
    stream.read(reinterpret_cast<char*>(&length), sizeof(length));
    length = valueConverter::swapUInt16(length);
    return true;
}

bool JPEGReader::readSegment(const std::uint16_t length, std::vector<char> &data){
    stream.read(data.data(), length - 4);
    return true;
}

bool JPEGReader::readAPP0(stAPP0 &app0){
    enSegmentMarker mark = enSegmentMarker::APP0;
    std::uint16_t marker;
    getMarker(marker);
    if (mark != marker) return false;
    //
//    stream.read(reinterpret_cast<char*>(&app0.length), sizeof(app0.length));
//    app0.length = valueConverter::swapUInt16(app0.length);
    //
    stream.read(reinterpret_cast<char*>(&app0.JFIFIdentifier), sizeof(app0.JFIFIdentifier));


    return true;
}