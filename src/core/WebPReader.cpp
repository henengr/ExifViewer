#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>

#include "WebPReader.hpp"


std::int32_t WebPReader::readHeader(std::istream &stream, stWebP_VP8XHeader &header){
    if (!stream) return FILEREADER_FILE_NOT_OPENED;

    stream.clear();
    stream.seekg(0, std::ios::beg);

    constexpr std::uint32_t headSize = 12 + 18;
    char buf[headSize];
    stream.read(buf, headSize);
    if (stream.gcount() < headSize) return FILEREADER_INVALID_FILE_SIZE;

    if (std::memcmp(buf, "RIFF", 4) != 0) return FILEREADER_INVALID_FILE_FORMAT;
    header.fourCC = "RIFF";
    header.fileSize = *reinterpret_cast<const std::uint32_t*>(&buf[4]);
    header.webP = "WEBP";
    header.chunkHeader = "VP8X";
    header.payloadSize = *reinterpret_cast<const std::uint32_t*>(&buf[16]);
    header.flagICCProfile = (buf[20] & 0b00100000) != 0;
    header.flagAlpha      = (buf[20] & 0b00010000) != 0;
    header.flagExif       = (buf[20] & 0b00001000) != 0;
    header.flagXMP        = (buf[20] & 0b00000100) != 0;
    header.flagAnimation  = (buf[20] & 0b00000010) != 0;
    header.canvasWidth  = static_cast<std::uint32_t>((static_cast<std::uint8_t>(buf[24])) | 
                                                     (static_cast<std::uint8_t>(buf[25]) << 8) | 
                                                     (static_cast<std::uint8_t>(buf[26]) << 16)) + 1;
    header.canvasHeight = static_cast<std::uint32_t>((static_cast<std::uint8_t>(buf[27])) | 
                                                     (static_cast<std::uint8_t>(buf[28]) << 8) | 
                                                     (static_cast<std::uint8_t>(buf[29]) << 16)) + 1;

    return headSize;
}

std::int32_t WebPReader::readChunk(const std::string fourCC, std::istream &stream, std::vector<std::uint8_t> &data){
    // https://developers.google.com/speed/webp/docs/riff_container?hl=en
    char fCC[4];
    std::uint32_t paySize;
    bool found = false;
    while (stream.read(fCC, 4)){
        if (stream.gcount() < 4) return FILEREADER_INVALID_FILE_SIZE;
        if (std::memcmp(fCC, fourCC.c_str(), 4) != 0){      // Skip over fourCC that is not the target
            stream.read(reinterpret_cast<char*>(&paySize), 4);
            if (stream.gcount() < 4) return FILEREADER_INVALID_FILE_SIZE;
            if (paySize % 2 != 0) ++paySize;    // If the value is odd, there is a 1-byte padding and add that amount
            stream.seekg(paySize, std::ios::cur);   // Skip the payload
            if (stream.eof()) return FILEREADER_INVALID_FILE_SIZE;
            continue;
        }

        found = true;
        break;
    }
    if (!found) return FILEREADER_EXIF_BLOCK_NOT_FOUND;

    stream.read(reinterpret_cast<char*>(&paySize), 4);
    if (stream.gcount() < 4) return FILEREADER_INVALID_FILE_SIZE;
    data.resize(paySize);
    stream.read(reinterpret_cast<char*>(data.data()), paySize);
    if (stream.gcount() < paySize || stream.eof()) return FILEREADER_INVALID_FILE_SIZE;

    return paySize + 8;
}


enEndian WebPReader::getEndian() const{
    return endian;
}

std::int32_t WebPReader::getImageInfo(stImageInfo &info){
    std::istream &stream = *arstream;
    if (!stream){
        return FILEREADER_FILE_NOT_OPENED;
    }

    stWebP_VP8XHeader header;
    std::int32_t ret;
    ret = readHeader(stream, header);
    if (ret < 0) return ret;

    info.width = header.canvasWidth;
    info.height = header.canvasHeight;
    info.bitsPerPixel = 0;

    return ret;
}

std::int32_t WebPReader::parse(std::vector<std::uint8_t> &data){
    std::istream &stream = *arstream;
    stWebP_VP8XHeader header;
    std::int32_t ret;

    ret = readHeader(stream, header);
    if (ret < 0) return ret;
    if (!header.flagExif) return FILEREADER_EXIF_BLOCK_NOT_FOUND;

    ret = readChunk("EXIF", stream, data);

    if (data[0] == 'M' && data[1] == 'M'){
        endian = enEndian::bigEndian;
    }else if (data[0] == 'I' && data[1] == 'I'){
        endian = enEndian::littleEndian;
    }

    return ret;
}


