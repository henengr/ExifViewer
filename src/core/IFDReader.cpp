#include "IFDReader.hpp"
#include "JPEGReader.hpp"


std::int32_t IFDReader::readIFD(const std::uint32_t offset, const enEndian endian, const std::vector<std::uint8_t> &data,
                stIFD &ifd){
    std::uint32_t iOffset = offset;
    if (iOffset + 12 > data.size()) return -1;
    //
    ifd.tag = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    iOffset += sizeof(ifd.tag);
    ifd.type = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    iOffset += sizeof(ifd.type);
    ifd.count = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
            (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
            (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
    iOffset += sizeof(ifd.count);
    std::uint32_t iOfs = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
            (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
            (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
    if (endian == enEndian::littleEndian){
        ifd.tag = valueConverter::swapUInt16(ifd.tag);
        ifd.type = valueConverter::swapUInt16(ifd.type);
        ifd.count = valueConverter::swapUInt32(ifd.count);
        iOfs = valueConverter::swapUInt32(iOfs);
    }
    //
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::BYTE) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::ASCII) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::UNDEFINED)){
        if (ifd.count <= 4){
            auto is = data.begin() + iOffset;
            auto ie = is + ifd.count;
            ifd.data.assign(is, ie);
        } else {
            if (iOfs + ifd.count > data.size()) return -1;
            auto is = data.begin() + iOfs;
            auto ie = is + ifd.count;
            ifd.data.assign(is, ie);
        }
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::SHORT)){
        if (ifd.count <= 2){
            auto is = data.begin() + iOffset;
            auto ie = is + ifd.count * 2;
            ifd.data.assign(is, ie);
        } else {
            if (iOfs + ifd.count * 2 > data.size()) return -1;
            auto is = data.begin() + iOfs;
            auto ie = is + ifd.count * 2;
            ifd.data.assign(is, ie);
        }
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::LONG) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::SLONG)){
        if (ifd.count <= 1){
            auto is = data.begin() + iOffset;
            auto ie = is + ifd.count * 4;
            ifd.data.assign(is, ie);
        } else {
            if (iOfs + ifd.count * 4 > data.size()) return -1;
            auto is = data.begin() + iOfs;
            auto ie = is + ifd.count * 4;
            ifd.data.assign(is, ie);
        }
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::RATIONAL) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::SRATIONAL)){
        if (iOfs + ifd.count * 8 > data.size()) return -1;
        auto is = data.begin() + iOfs;
        auto ie = is + ifd.count * 8;
        ifd.data.assign(is, ie);
    }
    //
    iOffset += sizeof(iOfs);
    return static_cast<std::int32_t>(iOffset);
}

