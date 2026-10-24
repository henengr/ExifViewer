

#include "JXLReader.hpp"

std::int32_t JXLReader::readBox(std::string boxName, std::istream &stream, std::vector<std::uint8_t> &data){
    // https://www.iso.org/standard/91379.html  -- ISO/IEC 18181-2026
    char sizeBytes[4];
    std::streampos readPos = 0;
    while (stream.read(sizeBytes, 4)){
        std::streampos startPos = stream.tellg() - static_cast<std::streamoff>(4);
        if (stream.gcount() < 4) return FILEREADER_INVALID_FILE_SIZE;

        std::uint64_t boxSize = static_cast<std::uint32_t>((static_cast<std::uint8_t>(sizeBytes[0]) << 24) |
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

        stream.clear();
        stream.seekg(nextBoxPos);
        readPos = stream.tellg();

        break;
    }
    return static_cast<std::uint32_t>(readPos);
}

std::int32_t JXLReader::parse(std::vector<std::uint8_t> &data){
    std::istream &stream = *_pstream;

    if (!stream){
        return FILEREADER_FILE_NOT_OPENED;
    }
    stream.clear();
    stream.seekg(0, std::ios::beg);

    // Read header
    auto pbuf = std::make_unique<std::vector<std::uint8_t>>();
    std::int32_t ret = readBox("JXL ", stream, *pbuf);
    if (ret < 0){
        return FILEREADER_INVALID_FILE_FORMAT;
    }
    pbuf->clear();

    ret = readBox("ftyp", stream, *pbuf);
    if (ret < 0){
        return FILEREADER_INVALID_FILE_FORMAT;
    }
    pbuf->clear();

    ret = readBox("Exif", stream, data);
    if (ret < 0){
        return FILEREADER_EXIF_BLOCK_NOT_FOUND;
    }
    if (data.size() < 4) return FILEREADER_INVALID_FILE_SIZE;
    data.erase(data.begin(), data.begin() + 4);     // Erase first 4 bytes

    return ret;
}

