#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>

#include "PNGReader.hpp"


std::int32_t PNGReader::readChunk(const std::string fourCC, std::istream &stream, std::vector<std::uint8_t> &data){
    // https://www.libpng.org/pub/png/spec/1.2/PNG-Contents.html
    char fCC[4];
    char psize[4];
    std::uint32_t paySize;
    std::uint32_t crc;
    bool found = false;
    while (stream.read(psize, 4)){
        if (stream.gcount() < 4) return FILEREADER_INVALID_FILE_SIZE;
        paySize = static_cast<std::uint32_t>((static_cast<std::uint8_t>(psize[3])) |
                                              (static_cast<std::uint8_t>(psize[2]) << 8) |
                                              (static_cast<std::uint8_t>(psize[1]) << 16) |
                                              (static_cast<std::uint8_t>(psize[0]) << 24));
        if (paySize == 0) return FILEREADER_INVALID_FILE_FORMAT;

        stream.read(fCC, 4);
        if (stream.gcount() < 4) return FILEREADER_INVALID_FILE_SIZE;
        if (std::memcmp(fCC, fourCC.c_str(), 4) != 0){
            stream.seekg(paySize, std::ios::cur);
            if (stream.eof()) return FILEREADER_INVALID_FILE_SIZE;
            stream.read(reinterpret_cast<char*>(&crc), 4);
            if (stream.gcount() < 4 || stream.eof()) return FILEREADER_INVALID_FILE_SIZE;
            continue;
        }

        found = true;
        break;
    }
    if (!found) return FILEREADER_EXIF_BLOCK_NOT_FOUND;

    if (stream.gcount() < 4) return FILEREADER_INVALID_FILE_SIZE;
    paySize = static_cast<std::uint32_t>((static_cast<std::uint8_t>(psize[3])) |
                                         (static_cast<std::uint8_t>(psize[2]) << 8) |
                                         (static_cast<std::uint8_t>(psize[1]) << 16) |
                                         (static_cast<std::uint8_t>(psize[0]) << 24));
    data.resize(paySize);
    stream.read(reinterpret_cast<char*>(data.data()), paySize);
    if (stream.gcount() < paySize || stream.eof()) return FILEREADER_INVALID_FILE_SIZE;
    stream.read(reinterpret_cast<char*>(&crc), 4);
    if (stream.gcount() < 4 || stream.eof()) return FILEREADER_INVALID_FILE_SIZE;

    return paySize + 12;
}

std::int32_t PNGReader::getIHDRInfo(std::istream &stream, stPNG_IHDRData &ihdr){
    std::vector<std::uint8_t> data;

    std::int32_t ret = readChunk("IHDR", stream, data);
    ihdr.width  = static_cast<std::uint32_t>((static_cast<std::uint8_t>(data[3])) |
                                             (static_cast<std::uint8_t>(data[2]) << 8) |
                                             (static_cast<std::uint8_t>(data[1]) << 16) |
                                             (static_cast<std::uint8_t>(data[0]) << 24));
    ihdr.height = static_cast<std::uint32_t>((static_cast<std::uint8_t>(data[7])) |
                                             (static_cast<std::uint8_t>(data[6]) << 8) |
                                             (static_cast<std::uint8_t>(data[5]) << 16) |
                                             (static_cast<std::uint8_t>(data[4]) << 24));
    ihdr.bitDepth = data[8];
    ihdr.colorType = static_cast<enPNG_ColorType>(data[9]);
    ihdr.compressionMethod = static_cast<enPNG_CompressionMethod>(data[10]);
    ihdr.filterMethod = static_cast<enPNG_FilterMethod>(data[11]);
    ihdr.interlaceMethod = static_cast<enPNG_InterlaceMethod>(data[12]);

    return ret;
}


enEndian PNGReader::getEndian() const{
    return endian;
}

std::int32_t PNGReader::getImageInfo(stImageInfo &info){
    std::istream &stream = *arstream;
    if (!stream){
        return FILEREADER_FILE_NOT_OPENED;
    }
    stream.clear();
    stream.seekg(0, std::ios::beg);

    // Read header
    char head[8];
    stream.read(head, 8);
    if (stream.gcount() < 8) return FILEREADER_INVALID_FILE_SIZE;
    std::uint8_t header[] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    if (std::memcmp(head, header, 8) != 0) return FILEREADER_INVALID_FILE_FORMAT;

    stPNG_IHDRData ihdr;
    std::int32_t ret = getIHDRInfo(stream, ihdr);
    if (ret < 0) return ret;

    info.width = ihdr.width;
    info.height = ihdr.height;
    info.bitsPerPixel = ihdr.bitDepth;

    return ret;
}

std::int32_t PNGReader::parse(std::vector<std::uint8_t> &data){
    std::istream &stream = *arstream;
    if (!stream){
        return FILEREADER_FILE_NOT_OPENED;
    }
    stream.clear();
    stream.seekg(0, std::ios::beg);

    // Read header
    char head[8];
    stream.read(head, 8);
    if (stream.gcount() < 8) return FILEREADER_INVALID_FILE_SIZE;
    std::uint8_t header[] = {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a};
    if (std::memcmp(head, header, 8) != 0) return FILEREADER_INVALID_FILE_FORMAT;

    std::int32_t ret = readChunk("eXIf", stream, data);

    if (data[0] == 'M' && data[1] == 'M'){
        endian = enEndian::bigEndian;
    }else if (data[0] == 'I' && data[1] == 'I'){
        endian = enEndian::littleEndian;
    }

    return ret;
}

