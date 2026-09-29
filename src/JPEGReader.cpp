#include <string>
#include <vector>
#include <iomanip>
#include <cctype>
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
    uint64_t ret = 0LL;
    ifs.read(reinterpret_cast<char*>(&ret), sizeof(ret));
    return ret;
}

void valueConverter::dispDumb(std::vector<char> &data){
    std::ios::fmtflags f(std::cout.flags());
    std::size_t count = 0;
    std::string ascii = "";
    std::string header = "Offset    |  +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F  | 0123456789ABCDEF";
    std::cout << header << std::endl;
    for (std::vector<char>::iterator i = data.begin(); i != data.end(); ++i){
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


enEndian JPEGReader::getEndian() const {
    return endian;
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
    std::size_t readsize = length - 2;
    data.resize(readsize);
    stream.read(data.data(), readsize);
    return static_cast<std::size_t>(stream.gcount()) == readsize;
}

bool JPEGReader::getAPP0(const std::uint16_t length, stAPP0 &app0, const std::vector<char> data){
    //if (data.size() < length) return false;
    auto i = data.begin();
    std::size_t size = 0;
    //
    size = sizeof(app0.JFIFIdentifier);
    std::copy_n(i, size, app0.JFIFIdentifier);
    i += size;
    //
    size = sizeof(app0.version);
    std::copy_n(i, size, reinterpret_cast<char*>(&app0.version));
    i += size;
    //
    size = sizeof(app0.pixelConcentrationUnit);
    std::copy_n(i, size, reinterpret_cast<char*>(&app0.pixelConcentrationUnit));
    //
    return true;
}

bool JPEGReader::getAPP1(const std::uint16_t length, stAPP1 &app1, const std::vector<char> data){
    //if (data.size() < length) return false;
    auto i = data.begin();
    std::size_t size = 0;
    //
    size = sizeof(app1.ExifIdentifier);
    std::copy_n(i, size, app1.ExifIdentifier);
    i += size;
    //
    size = sizeof(app1.TIFFByteOrder);
    std::copy_n(i, size, app1.TIFFByteOrder);
    i += size;
    if (app1.TIFFByteOrder[0] == 'M' && app1.TIFFByteOrder[1] == 'M')
        endian = enEndian::bigEndian;
    else if (app1.TIFFByteOrder[0] == 'I' && app1.TIFFByteOrder[1] == 'I')
        endian = enEndian::littleEndian;
    //
    size = sizeof(app1.TIFFIdentifier);
    std::copy_n(i, size, reinterpret_cast<char*>(&app1.TIFFIdentifier));
    i += size;
    //
    size = sizeof(app1.IFD0thOffset);
    std::copy_n(i, size, reinterpret_cast<char*>(&app1.IFD0thOffset));
    i += size;
    //
    app1.IFDs.clear();
    app1.IFDs.insert(app1.IFDs.end(), i, data.end());
    //
    return true;
}

