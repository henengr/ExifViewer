#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>

#include "HEICReader.hpp"


enEndian HEICReader::getEndian() const{
    return endian;
}

std::int32_t HEICReader::parse(std::vector<std::uint8_t> &data){
    std::int32_t ret;
    std::ifstream &stream = arstream;

    if (!stream.is_open()){
        return FILEREADER_FILE_NOT_OPENED;
    }
    stream.clear();
    stream.seekg(0, std::ios::beg);

    std::uint8_t exif[6];
    bool exifFound = false;

    while (stream.read(reinterpret_cast<char*>(&exif), 1)){
        if (exif[0] != 'E') continue;

        stream.read(reinterpret_cast<char*>(&exif[1]), 5);
        if (stream.gcount() < 5) break;

        if (std::memcmp(exif, "Exif\0\0", 6) != 0) continue;

        char byteOrder[2];
        stream.read(byteOrder, 2);
        if (stream.gcount() < 2) break;

        if (byteOrder[0] == 'M' && byteOrder[1] == 'M'){
            endian = enEndian::bigEndian;
        }else if (byteOrder[0] == 'I' && byteOrder[1] == 'I'){
            endian = enEndian::littleEndian;
        }else{
            continue;   // It's not an Exif block
        }
        stream.seekg(-2, std::ios::cur);    // Return the position to the ByteOrder

        exifFound = true;
        break;
    }
    if (!exifFound) return FILEREADER_EXIF_BLOCK_NOT_FOUND;

    std::streamsize curPos = stream.tellg();    // Save the current position
    stream.seekg(0, std::ios::end);
    std::streamsize endPos = stream.tellg();    // Save the end position
    stream.seekg(curPos, std::ios::beg);
    data.resize(endPos - curPos);

    this->endian = endian;
    stream.read(reinterpret_cast<char*>(data.data()), endPos - curPos);

    return static_cast<std::int32_t>(endPos - curPos);
}


