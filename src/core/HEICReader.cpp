#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>

#include "HEICReader.hpp"


/// @brief Find the specified Boxes
/// @param[in]      boxName     The name of Boxes
/// @param[in,out]  stream      Opened stream
/// @param[out]     datas       An array that returns the payloads
/// @return                     Number of found Boxes
std::int32_t HEICReader::readBoxes(std::string boxName, std::istream &stream, std::vector<std::vector<std::uint8_t>> &datas){
    // https://www.iso.org/standard/85596.html  -- ISO/IEC 14496-12:2026
    // https://www.iso.org/standard/89035.html  -- ISO/IEC 23008-12:2025

    char sizeBytes[4];
    std::uint64_t boxSize = 0;
    std::int32_t foundCount = 0;
    while (stream.read(sizeBytes, 4)){
        std::streampos startPos = stream.tellg() - static_cast<std::streamoff>(4);
        if (stream.gcount() < 4) return FILEREADER_INVALID_FILE_SIZE;

        boxSize = static_cast<std::uint32_t>((static_cast<std::uint8_t>(sizeBytes[0]) << 24) |
                                             (static_cast<std::uint8_t>(sizeBytes[1]) << 16) |
                                             (static_cast<std::uint8_t>(sizeBytes[2]) << 8) |
                                             (static_cast<std::uint8_t>(sizeBytes[3])));
        
        char typeBytes[4];
        stream.read(typeBytes, 4);
        if (stream.gcount() < 4) return FILEREADER_INVALID_FILE_SIZE;
        std::string actBoxType(typeBytes, 4);

        std::uint64_t headerSize = 8;

        if (boxSize == 1){
            char largeBytes[8];
            stream.read(largeBytes, 8);
            if (stream.gcount() < 8) return FILEREADER_INVALID_FILE_SIZE;

            boxSize = 0;
            for (int i = 0; i < 8; ++i){
                boxSize = (boxSize << 8) | static_cast<std::uint8_t>(largeBytes[i]);
            }
            headerSize += 8;
        }else if (boxSize == 0){
            // If boxSize is 0, it means the entire remainder of the file, up to the end, constitutes the payload.
            stream.clear();
            std::streampos curPos = stream.tellg();
            stream.seekg(0, std::ios::end);
            boxSize = stream.tellg() - startPos;
            stream.seekg(curPos);
        }
        if (boxSize < headerSize) return FILEREADER_INVALID_FILE_SIZE;

        std::uint64_t payloadSize = boxSize - headerSize;
        std::streampos nextBoxPos = startPos + static_cast<std::streamoff>(boxSize);

        if (actBoxType == "meta" || actBoxType == "iprp" || actBoxType == "ipco"){
            if (actBoxType == "meta") {
                stream.seekg(4, std::ios::cur);
                payloadSize -= 4;
            }

            std::int32_t ret = readBoxes(boxName, stream, datas);
            if (ret > 0){
                foundCount += ret;
            }
            stream.clear();
            stream.seekg(nextBoxPos);
            continue;
        }

        if (actBoxType != boxName){
            if (payloadSize > 0) {
                stream.seekg(payloadSize, std::ios::cur);
            }
            continue;
        }

        // get payloads
        std::vector<std::uint8_t> currentPayload(payloadSize);
        if (payloadSize > 0){
            stream.read(reinterpret_cast<char*>(currentPayload.data()), payloadSize);
            if (stream.gcount() < static_cast<std::streamsize>(payloadSize)) return FILEREADER_INVALID_FILE_SIZE;
        }
        datas.push_back(std::move(currentPayload));
        foundCount++;

        stream.clear();
        stream.seekg(nextBoxPos);
    }

    return foundCount;
}


/// @brief Parse the HEIC image file
/// @param[out]     data    Return the payload
/// @return                 Number of read data
std::int32_t HEICReader::parse(std::vector<std::uint8_t> &data){
    std::istream &stream = *_pstream;

    if (!stream){
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
            _endian = enEndian::bigEndian;
        }else if (byteOrder[0] == 'I' && byteOrder[1] == 'I'){
            _endian = enEndian::littleEndian;
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

    stream.read(reinterpret_cast<char*>(data.data()), endPos - curPos);

    _pImageInfo = std::make_unique<stImageInfo>();
    std::vector<std::vector<std::uint8_t>> bufs;
    stream.clear();
    stream.seekg(0, std::ios::beg);
    std::int32_t ret;
    ret = readBoxes("ispe", stream, bufs);

    std::uint32_t mWidth = 0;
    std::uint32_t mHeight = 0;
    for (const auto buf: bufs){
        // Define the image size as the maximum value obtained by multiplying the image's width and height
        if (buf.size() < 12) return FILEREADER_INVALID_FILE_FORMAT;
        std::uint32_t w = static_cast<std::uint32_t>((static_cast<std::uint8_t>(buf[4]) << 24) |
                                                     (static_cast<std::uint8_t>(buf[5]) << 16) |
                                                     (static_cast<std::uint8_t>(buf[6]) << 8) |
                                                     (static_cast<std::uint8_t>(buf[7])));
        std::uint32_t h = static_cast<std::uint32_t>((static_cast<std::uint8_t>(buf[8]) << 24) |
                                                     (static_cast<std::uint8_t>(buf[9]) << 16) |
                                                     (static_cast<std::uint8_t>(buf[10]) << 8) |
                                                     (static_cast<std::uint8_t>(buf[11])));
        if (w * h > mWidth * mHeight){
            mWidth = w;
            mHeight = h;
        }
    }
    _pImageInfo->width = mWidth;
    _pImageInfo->height = mHeight;

    bufs.clear();
    stream.clear();
    stream.seekg(0, std::ios::beg);
    ret = readBoxes("pixi", stream, bufs);

    std::uint32_t mBits = 0;
    for (const auto buf: bufs){
        std::uint8_t ch = buf[4];
        std::uint8_t bitDepth = 0;
        if (buf.size() < 6) return FILEREADER_INVALID_FILE_FORMAT;
        bitDepth = buf[5];      // Set the value as the bit depth for the first channel
        if (bitDepth > mBits){
            mBits = bitDepth;
        }
    }
    _pImageInfo->bitsPerPixel = mBits;

    return static_cast<std::int32_t>(endPos - curPos);
}

