#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstring>

#include "HEICReader.hpp"


std::int32_t readBox(std::string boxName, std::istream &stream, std::vector<std::uint8_t> &data){
    data.clear();
    stream.clear();

    char sizeBytes[4];
    std::uint64_t boxSize = 0;
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

        if (boxName == "meta" || boxName == "iprp" || boxName == "ipco"){
            if (actBoxType == "meta") {
                stream.seekg(4, std::ios::cur);
                payloadSize -= 4;
            }

            std::int32_t ret = readBox(boxName, stream, data);
            if (ret > 0){
                return ret;
            }
            continue;
        }

        if (actBoxType != boxName){
            if (payloadSize > 0) {
                stream.seekg(payloadSize, std::ios::cur);
            }
            continue;
        }

        // get payload
        data.resize(payloadSize);
        if (payloadSize > 0){
            stream.read(reinterpret_cast<char*>(data.data()), payloadSize);
            if (stream.gcount() < static_cast<std::streamsize>(payloadSize)) return FILEREADER_INVALID_FILE_SIZE;
        }

        return static_cast<std::int32_t>(boxSize);
    }

    return 0;   // Specified box doesn't exist
}


/*std::int32_t HEICReader::getImageInfo(stImageInfo &info){
    std::istream &stream = *_pstream;
    if (!stream){
        return FILEREADER_FILE_NOT_OPENED;
    }

    info.width = 0;
    info.height = 0;
    info.bitsPerPixel = 0;

    return 1;
}*/

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

    return static_cast<std::int32_t>(endPos - curPos);
}


