#include <string>
#include <vector>
#include <iomanip>
#include <cstring>
#include <algorithm>

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

std::uint8_t valueConverter::streamToUInt8(std::istream &ifs){
    uint8_t ret = 0;
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(ret));
    return ret;
}

std::uint16_t valueConverter::streamToUInt16(std::istream &ifs){
    uint16_t ret = 0;
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(ret));
    return ret;
}

std::uint32_t valueConverter::streamToUInt32(std::istream &ifs){
    uint32_t ret = 0;
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(ret));
    return ret;
}

std::uint64_t valueConverter::streamToUInt64(std::istream &ifs){
    uint64_t ret = 0ULL;
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(ret));
    return ret;
}

void valueConverter::dispDumb(std::vector<std::uint8_t> &data){
    std::ios::fmtflags f(std::cout.flags());
    std::size_t count = 0;
    std::string ascii = "";
    std::string header = "Offset    |  +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F  | 0123456789ABCDEF";
    std::cout << header << std::endl;
    for (std::vector<std::uint8_t>::iterator i = data.begin(); i != data.end(); ++i){
        std::uint8_t byte = static_cast<std::uint8_t>(*i);
        if (count % 16 == 0){
            std::cout << "+" << std::hex << std::setw(8) << std::setfill('0') << count << " |  ";
        }
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

std::int32_t JPEGReader::readSegment(std::uint16_t marker, std::istream &stream, std::vector<std::uint8_t> &data){
    std::uint8_t byte;
    std::uint16_t mk;
    std::uint16_t len;

    stream.clear();
    stream.seekg(0, std::ios::beg);
    do {
        stream.read(reinterpret_cast<char*>(&mk), sizeof(mk));
        if (stream.gcount() < sizeof(mk) || stream.eof()){
            return JPEGREADER_INVALID_FILE_SIZE;
        }
        mk = valueConverter::swapUInt16(mk);

        if (mk != marker) continue;

        // These segments have no length field or data
        if (mk == static_cast<std::uint16_t>(enSegmentMarker::ByteStuffing) ||
            mk == static_cast<std::uint16_t>(enSegmentMarker::SOI) || 
            mk == static_cast<std::uint16_t>(enSegmentMarker::EOI) ||
            mk == static_cast<std::uint16_t>(enSegmentMarker::TEM) ||
            (mk >= static_cast<std::uint16_t>(enSegmentMarker::RST0) && mk <= static_cast<std::uint16_t>(enSegmentMarker::RST7))) {
            data.clear();
            return 0;
        }

        stream.read(reinterpret_cast<char*>(&len), sizeof(len));
        if (stream.gcount() < sizeof(len) || stream.eof()){
            return JPEGREADER_INVALID_FILE_FORMAT;
        }
        len = valueConverter::swapUInt16(len);
        std::uint16_t payloadLen = len - sizeof(mk);

        if (payloadLen <= 0) return JPEGREADER_INVALID_FILE_FORMAT;

        data.resize(payloadLen);
        stream.read(reinterpret_cast<char*>(data.data()), payloadLen);
        if (stream.gcount() < payloadLen || stream.eof()){
            return JPEGREADER_INVALID_FILE_FORMAT;
        }
        break;
    } while (!stream.eof());

    return stream.gcount();
}

std::int32_t JPEGReader::parseAPP0(const std::vector<std::uint8_t>& data, stAPP0 &value){
    std::uint32_t ofs = 0;
    if (data.size() < 12) return JPEGREADER_INVALID_FILE_SIZE;

    auto is = data.begin();
    auto ie = is + 5;
    std::string jfif(is, ie);       // JFIF
    std::memcpy(value.JFIFIdentifier, data.data(), 5);
    ofs += 5;
    if (std::memcmp(value.JFIFIdentifier, "JFIF\0", 5) != 0)
        return JPEGREADER_INVALID_FILE_FORMAT;

    value.version = (static_cast<std::uint16_t>(data[ofs]) << 8) | data[ofs + 1];
    ofs += sizeof(value.version);

    value.pixelConcentrationUnit = data[ofs];
    ofs++;

    value.pixelConcentrationX = (static_cast<std::uint16_t>(data[ofs]) << 8) | data[ofs + 1];
    ofs += sizeof(value.pixelConcentrationX);
    value.pixelConcentrationY = (static_cast<std::uint16_t>(data[ofs]) << 8) | data[ofs + 1];
    ofs += sizeof(value.pixelConcentrationY);

    if (data.size() < 14) return ofs;

    value.thumbnailImageWidth = data[ofs];
    value.thumbnailImageHeight = data[ofs + 1];
    ofs += 2;

    std::uint32_t payloadLen = data.size() - ofs;
    if (payloadLen <= 0) return ofs;

    value.thumbnailImage.clear();
    value.thumbnailImage.resize(payloadLen);
    auto tb = value.thumbnailImage.begin();
    auto db = data.begin() + ofs;
    std::copy_n(db, payloadLen, tb);

    return ofs;
}

std::int32_t JPEGReader::parseAPP1(const std::vector<std::uint8_t>& data, enEndian &endian, stAPP1 &value){
    std::uint32_t ofs = 0;
    if (data.size() < 16) return JPEGREADER_INVALID_FILE_SIZE;

    auto is = data.begin();
    auto ie = is + 6;
    std::string exif(is, ie);       // Exif
    std::memcpy(value.ExifIdentifier, data.data(), 6);
    ofs += 6;
    if (std::memcmp(value.ExifIdentifier, "Exif\0\0", 6) != 0)
        return JPEGREADER_INVALID_FILE_FORMAT;

    if (data[ofs] == 'M' && data[ofs + 1] == 'M')
        endian = enEndian::bigEndian;
    else if (data[ofs] == 'I' && data[ofs + 1] == 'I')
        endian = enEndian::littleEndian;
    else return JPEGREADER_INVALID_FILE_FORMAT;
    //ofs += 2;     Start the payload from "ByteOrder".

    std::uint32_t payloadLen = data.size() - ofs;
    if (payloadLen <= 0) return JPEGREADER_INVALID_FILE_FORMAT;     // No IFD data.

    value.data.clear();
    value.data.resize(payloadLen);
    auto tb = value.data.begin();
    auto db = data.begin() + ofs;
    std::copy_n(db, payloadLen, tb);

    return ofs;
}


enEndian JPEGReader::getEndian() const{
    return endian;
}

std::int32_t JPEGReader::readSegment(std::uint16_t marker){
    return readSegment(marker, arstream, ardata);
}

std::int32_t JPEGReader::parseAPP0(stAPP0 &value){
    return parseAPP0(ardata, value);
}

std::int32_t JPEGReader::parseAPP1(enEndian &endian, stAPP1 &value){
    return parseAPP1(ardata, endian, value);
}
