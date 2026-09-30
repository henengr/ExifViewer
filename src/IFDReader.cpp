#include "IFDReader.hpp"
#include "JPEGReader.hpp"


constexpr std::uint16_t APP1HeaderSize = sizeof(stAPP1::TIFFByteOrder) + sizeof(stAPP1::TIFFIdentifier) +
    sizeof(stAPP1::IFD0thOffset);

bool IFDReader::getIFDValueByte(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, std::vector<std::uint8_t> &value){
    std::uint32_t iOffset = offset;
    if (iOffset + 12 > data.size()) return false;
    //
    std::uint16_t iTag = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    iOffset += sizeof(stIFDTag::tag);
    //
    std::uint16_t iType = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    iOffset += sizeof(stIFDTag::type);
    //
    std::uint32_t iCount = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
            (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
            (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
    iOffset += sizeof(stIFDTag::count);
    if (endian == enEndian::littleEndian){
        iTag = valueConverter::swapUInt16(iTag);
        iType = valueConverter::swapUInt16(iType);
        iCount = valueConverter::swapUInt32(iCount);
    }
    if (tag != iTag) return false;
    if (iType != static_cast<std::uint16_t>(enIFDType::BYTE)) return false;
    if (iOffset + iCount > data.size()) return false;
    //
    value.clear();  // initialize the vector
    if (iCount <= 4){   // If the number of elements is 4 or less, retrieve the value from the "Offset" value
        for (int i = 0; i < iCount; i++){
            value.push_back(data[iOffset + i]);
        }
    }else{  // If the number of elements is 5 or greater, retrieve the value at the location indicated by the Offset value
        std::uint32_t ofs = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
                (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
                (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
        if (endian == enEndian::littleEndian) ofs = valueConverter::swapUInt32(ofs);
        if (ofs + iCount > data.size()) return false;
        for (int i = 0; i < iCount; i++){
            value.push_back(data[ofs + i]);
        }
    }
    //
    return true;
};

bool IFDReader::getIFDValueShort(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, std::vector<std::uint16_t> &value){
    std::uint32_t iOffset = offset;
    if (iOffset + 12 > data.size()) return false;
    //
    std::uint16_t iTag = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    iOffset += sizeof(stIFDTag::tag);
    //
    std::uint16_t iType = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    iOffset += sizeof(stIFDTag::type);
    //
    std::uint32_t iCount = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
            (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
            (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
    iOffset += sizeof(stIFDTag::count);
    if (endian == enEndian::littleEndian){
        iTag = valueConverter::swapUInt16(iTag);
        iType = valueConverter::swapUInt16(iType);
        iCount = valueConverter::swapUInt32(iCount);
    }
    if (tag != iTag) return false;
    if (iType != static_cast<std::uint16_t>(enIFDType::SHORT)) return false;
    if (iOffset + iCount * 2 > data.size()) return false;
    //
    value.clear();  // initialize the vector
    if (iCount <= 4){   // If the number of elements is 4 or less, retrieve the value from the "Offset" value
        for (int i = 0; i < iCount; i++){
            std::uint8_t b1 = static_cast<std::uint8_t>(data[iOffset + i * 2]);
            std::uint8_t b2 = static_cast<std::uint8_t>(data[iOffset + i * 2 + 1]);
            std::uint16_t ret = (static_cast<std::uint16_t>(b1) << 8) | b2;
            if (endian == enEndian::littleEndian) ret = valueConverter::swapUInt16(ret);
            value.push_back(ret);
        }
    }else{  // If the number of elements is 5 or greater, retrieve the value at the location indicated by the Offset value
        std::uint32_t ofs = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
                (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
                (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
        if (endian == enEndian::littleEndian) ofs = valueConverter::swapUInt32(ofs);
        if (ofs + iCount * 2 > data.size()) return false;
        for (int i = 0; i < iCount; i++){
            std::uint8_t b1 = static_cast<std::uint8_t>(data[ofs + i * 2]);
            std::uint8_t b2 = static_cast<std::uint8_t>(data[ofs + i * 2 + 1]);
            std::uint16_t ret = (static_cast<std::uint16_t>(b1) << 8) | b2;
            if (endian == enEndian::littleEndian) ret = valueConverter::swapUInt16(ret);
            value.push_back(ret);
        }
    }
    //
    return true;
};


bool IFDReader::getIFDValue(const std::uint16_t tag, const std::uint32_t offset, stIFDTypeValue &value){
    std::uint32_t iOffset = offset - APP1HeaderSize;
    std::uint16_t iTag = (ardata[iOffset] << 8) | ardata[iOffset + 1];
    iOffset += sizeof(stIFDTag::tag);
    //
    std::uint16_t iType = (ardata[iOffset] << 8) | ardata[iOffset + 1];
    iOffset += sizeof(stIFDTag::type);
    //
    std::uint32_t iCount = (ardata[iOffset] << 24) | (ardata[iOffset + 1] << 16) |
            (ardata[iOffset + 2] << 8) | ardata[iOffset + 3];
    iOffset += sizeof(stIFDTag::count);
    //
    for (stIFDTag ifd: IFDTable){
        if (enendian == enEndian::littleEndian){
            iTag = valueConverter::swapUInt16(iTag);
            iType = valueConverter::swapUInt16(iType);
        }
        if (tag == iTag){
            if (static_cast<enIFDType>(iType) == enIFDType::BYTE){
                value.type = enIFDType::BYTE;
                value.byteValue = ardata[iOffset];
                if (enendian == enEndian::littleEndian) value.byteValue =
                        static_cast<std::uint8_t>(valueConverter::swapUInt32(value.byteValue));
            }
            if (static_cast<enIFDType>(iType) == enIFDType::ASCII){
                value.type = enIFDType::ASCII;
            }
            if (static_cast<enIFDType>(iType) == enIFDType::SHORT){
                value.type = enIFDType::SHORT;
                value.shortValue = (ardata[iOffset] << 8) | ardata[iOffset + 1];
                if (enendian == enEndian::littleEndian) value.shortValue = 
                        static_cast<std::uint16_t>(valueConverter::swapUInt32(value.shortValue));
            }
            if (static_cast<enIFDType>(iType) == enIFDType::LONG){
                value.type = enIFDType::LONG;
                value.shortValue = (ardata[iOffset] << 24) | (ardata[iOffset + 1] << 16) |
                        (ardata[iOffset + 2] << 8) | ardata[iOffset + 3];
                if (enendian == enEndian::littleEndian) value.longValue = 
                        static_cast<std::uint32_t>(valueConverter::swapUInt32(value.longValue));
            }
            if (static_cast<enIFDType>(iType) == enIFDType::RATIONAL){
                value.type = enIFDType::RATIONAL;
            }
            if (static_cast<enIFDType>(iType) == enIFDType::UNDEFINED){
                value.type = enIFDType::UNDEFINED;
            }
            if (static_cast<enIFDType>(iType) == enIFDType::SLONG){
                value.type = enIFDType::SLONG;
                value.slongValue = (ardata[iOffset] << 24) | (ardata[iOffset + 1] << 16) |
                        (ardata[iOffset + 2] << 8) | ardata[iOffset + 3];
                if (enendian == enEndian::littleEndian) value.slongValue = 
                        static_cast<std::int32_t>(valueConverter::swapUInt32(value.slongValue));
            }
            if (static_cast<enIFDType>(iType) == enIFDType::SRATIONAL){
                value.type = enIFDType::SRATIONAL;
            }
            break;
        }
        iOffset += sizeof(stIFDTag::offset);
    }
    //
    return true;
}