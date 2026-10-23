#include <string>
#include <vector>
#include <cstdint>
#include <iostream>
#include <fstream>

#include "TIFFReader.hpp"
#include "IFDReader.hpp"


/// @brief Parse the TIFF image file
/// @param[out]     data    Return the payload
/// @return                 Number of read data
std::int32_t TIFFReader::parse(std::vector<std::uint8_t> &data){
    std::istream &stream = *_pstream;

    if (!stream){
        return FILEREADER_FILE_NOT_OPENED;
    }
    stream.clear();
    stream.seekg(0, std::ios::beg);

    std::uint8_t byteOrder[2];
    stream.read(reinterpret_cast<char*>(&byteOrder), 2);
    if (stream.gcount() < 2) return FILEREADER_INVALID_FILE_SIZE;

    if (byteOrder[0] == 'M' && byteOrder[1] == 'M'){
        _endian = enEndian::bigEndian;
    }else if (byteOrder[0] == 'I' && byteOrder[1] == 'I'){
        _endian = enEndian::littleEndian;
    }else{
        return FILEREADER_INVALID_FILE_FORMAT;
    }

    stream.seekg(0, std::ios::end);
    std::streampos endPos = stream.tellg();
    stream.seekg(0, std::ios::beg);     // Return the position to the top of file

    data.resize(static_cast<std::size_t>(endPos));
    stream.read(reinterpret_cast<char*>(data.data()), endPos);
    if (stream.gcount() < static_cast<std::streamsize>(endPos))
        return FILEREADER_INVALID_FILE_SIZE;

    // Get image width, height and bit depth
    stream.clear();
    stream.seekg(0, std::ios::beg);

    auto rifd = std::make_unique<IFDReader>(data);
    std::vector<stIFD> ifds;
    rifd->readIFDs(ifds);

    _pImageInfo = std::make_unique<stImageInfo>();
    for (const auto &ifd: ifds){
        stIFDValue vals;
        IFDReader::getVariantIFD(ifd, _endian, vals);

        if (ifd.tag == 0x0100){     // ImageWidth
            if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SHORT)){
                std::vector<std::uint16_t> v = std::get<std::vector<std::uint16_t>>(vals.values);
                if (v.size() > 0){
                    _pImageInfo->width = static_cast<std::uint32_t>(v[0]);
                }
            }
            if (vals.IFD.type == static_cast<uint16_t>(enIFDType::LONG)){
                std::vector<std::uint32_t> v = std::get<std::vector<std::uint32_t>>(vals.values);
                if (v.size() > 0){
                    _pImageInfo->width = v[0];
                }
            }
        }
        if (ifd.tag == 0x0101){     // ImageHeight
            if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SHORT)){
                std::vector<std::uint16_t> v = std::get<std::vector<std::uint16_t>>(vals.values);
                if (v.size() > 0){
                    _pImageInfo->height = static_cast<std::uint32_t>(v[0]);
                }
            }
            if (vals.IFD.type == static_cast<uint16_t>(enIFDType::LONG)){
                std::vector<std::uint32_t> v = std::get<std::vector<std::uint32_t>>(vals.values);
                if (v.size() > 0){
                    _pImageInfo->height = v[0];
                }
            }
        }
        if (ifd.tag == 0x0102){     // BitsPerSample
            if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SHORT)){
                std::vector<std::uint16_t> v = std::get<std::vector<std::uint16_t>>(vals.values);
                if (v.size() > 0){
                    _pImageInfo->bitsPerPixel = v[0];
                }
            }
        }
    }

    return static_cast<std::int32_t>(endPos);
}

