#include <string>
#include <vector>
#include <iomanip>
#include <cstring>
#include <algorithm>

#include "JFIF.hpp"
#include "JPEGReader.hpp"


std::int32_t JPEGReader::readSegment(std::uint16_t marker, std::istream &stream, std::vector<std::uint8_t> &data){
    std::uint8_t byte;
    //std::uint16_t mk;
    std::uint16_t len;

    stream.clear();
    stream.seekg(0, std::ios::beg);

    while (stream.read(reinterpret_cast<char*>(&byte), 1)) {
        if (byte == 0xff) {
            std::uint8_t markerCode;
            if (!stream.read(reinterpret_cast<char*>(&markerCode), 1)) return FILEREADER_INVALID_FILE_SIZE;

            if (markerCode == 0x00) continue;

            std::uint16_t mk = (0xff00 | markerCode);

            if (mk != marker) {
                if (mk == static_cast<std::uint16_t>(enSegmentMarker::SOI) || 
                    mk == static_cast<std::uint16_t>(enSegmentMarker::EOI) ||
                    mk == static_cast<std::uint16_t>(enSegmentMarker::TEM) ||
                    (mk >= static_cast<std::uint16_t>(enSegmentMarker::RST0) && mk <= static_cast<std::uint16_t>(enSegmentMarker::RST7))) {
                    continue;
                }

                std::uint16_t skipLen;
                if (stream.read(reinterpret_cast<char*>(&skipLen), sizeof(skipLen))) {
                    skipLen = valueConverter::swapUInt16(skipLen);
                    if (skipLen >= 2) {
                        // Since segment length includes its own length, 2 is subtracted.
                        stream.seekg(skipLen - 2, std::ios::cur); 
                    }
                } else {
                    return FILEREADER_INVALID_FILE_SIZE;
                }
                continue;
            }

            if (mk == static_cast<std::uint16_t>(enSegmentMarker::SOI) || 
                mk == static_cast<std::uint16_t>(enSegmentMarker::EOI) ||
                mk == static_cast<std::uint16_t>(enSegmentMarker::TEM) ||
                (mk >= static_cast<std::uint16_t>(enSegmentMarker::RST0) && mk <= static_cast<std::uint16_t>(enSegmentMarker::RST7))) {
                data.clear();
                return 0;
            }

            std::uint16_t len;
            stream.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (stream.gcount() < sizeof(len)) {
                return FILEREADER_INVALID_FILE_SIZE;
            }
            len = valueConverter::swapUInt16(len);

            if (len < 2) return FILEREADER_INVALID_FILE_FORMAT;

            std::uint16_t payloadLen = len - 2;

            data.resize(payloadLen);

            if (payloadLen > 0) {
                stream.read(reinterpret_cast<char*>(data.data()), payloadLen);
                if (stream.gcount() < payloadLen) {
                    return FILEREADER_INVALID_FILE_FORMAT;
                }
            }

            return static_cast<std::int32_t>(payloadLen);

        }

    }

    return FILEREADER_INVALID_FILE_FORMAT;
}

std::int32_t JPEGReader::parseAPP0(const std::vector<std::uint8_t>& data, stAPP0 &value){
    std::uint32_t ofs = 0;
    if (data.size() < 12) return FILEREADER_INVALID_FILE_SIZE;

    auto is = data.begin();
    auto ie = is + 5;
    std::string jfif(is, ie);       // JFIF
    std::memcpy(value.JFIFIdentifier, data.data(), 5);
    ofs += 5;
    if (std::memcmp(value.JFIFIdentifier, "JFIF\0", 5) != 0)
        return FILEREADER_INVALID_FILE_FORMAT;

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
    if (data.size() < 16) return FILEREADER_INVALID_FILE_SIZE;

    auto is = data.begin();
    auto ie = is + 6;
    std::string exif(is, ie);       // Exif
    std::memcpy(value.ExifIdentifier, data.data(), 6);
    ofs += 6;
    if (std::memcmp(value.ExifIdentifier, "Exif\0\0", 6) != 0)
        return FILEREADER_INVALID_FILE_FORMAT;

    if (data[ofs] == 'M' && data[ofs + 1] == 'M')
        endian = enEndian::bigEndian;
    else if (data[ofs] == 'I' && data[ofs + 1] == 'I')
        endian = enEndian::littleEndian;
    else return FILEREADER_INVALID_FILE_FORMAT;
    // ofs += 2;     Start the payload from "ByteOrder".

    std::uint32_t payloadLen = data.size() - ofs;
    if (payloadLen <= 0) return FILEREADER_INVALID_FILE_FORMAT;     // No IFD data.

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

std::int32_t JPEGReader::parse(std::vector<std::uint8_t> &data){
    std::istream &stream = *arstream;
    if (!stream){
        return FILEREADER_FILE_NOT_OPENED;
    }

    std::int32_t ret = readSegment(static_cast<std::uint16_t>(enSegmentMarker::APP1), stream, ardata);
    if (ret < 0){
        return ret;
    }

    ret = parseAPP1(ardata, endian, app1);
    if (ret < 0){
        return ret;
    }
    this->endian = endian;
    data = app1.data;

    return ret;
}


