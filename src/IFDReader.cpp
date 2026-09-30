#include "IFDReader.hpp"
#include "JPEGReader.hpp"


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

bool IFDReader::getIFDValueAscii(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, std::string &value){
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
    if (iType != static_cast<std::uint16_t>(enIFDType::ASCII)) return false;
    if (iOffset + iCount > data.size()) return false;
    //
    value.clear();  // initialize the string
    if (iCount <= 4){   // If the number of elements is 4 or less, retrieve the value from the "Offset" value
        for (int i = 0; i < iCount; i++){
            char c = static_cast<char>(data[iOffset + i]);
            if (c == '\0') break;
            value += c;
        }
    }else{  // If the number of elements is 5 or greater, retrieve the value at the location indicated by the Offset value
        std::uint32_t ofs = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
                (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
                (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
        if (endian == enEndian::littleEndian) ofs = valueConverter::swapUInt32(ofs);
        if (ofs + iCount > data.size()) return false;
        for (int i = 0; i < iCount; i++){
            char c = static_cast<char>(data[ofs + i]);
            if (c == '\0') break;
            value += c;
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
    if (iCount <= 2){   // If the number of elements is 2 or less, retrieve the value from the "Offset" value
        for (int i = 0; i < iCount; i++){
            std::uint8_t b1 = static_cast<std::uint8_t>(data[iOffset + i * 2]);
            std::uint8_t b2 = static_cast<std::uint8_t>(data[iOffset + i * 2 + 1]);
            std::uint16_t ret = (static_cast<std::uint16_t>(b1) << 8) | b2;
            if (endian == enEndian::littleEndian) ret = valueConverter::swapUInt16(ret);
            value.push_back(ret);
        }
    }else{  // If the number of elements is 3 or greater, retrieve the value at the location indicated by the Offset value
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

bool IFDReader::getIFDValueLong(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, std::vector<std::uint32_t> &value){
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
    if (iType != static_cast<std::uint16_t>(enIFDType::LONG)) return false;
    if (iOffset + iCount * 4 > data.size()) return false;
    //
    value.clear();  // initialize the vector
    if (iCount <= 1){   // If the number of elements is 1 or less, retrieve the value from the "Offset" value
        for (int i = 0; i < iCount; i++){
            std::uint8_t b1 = static_cast<std::uint8_t>(data[iOffset + i * 4]);
            std::uint8_t b2 = static_cast<std::uint8_t>(data[iOffset + i * 4 + 1]);
            std::uint8_t b3 = static_cast<std::uint8_t>(data[iOffset + i * 4 + 2]);
            std::uint8_t b4 = static_cast<std::uint8_t>(data[iOffset + i * 4 + 3]);
            std::uint32_t ret = (static_cast<std::uint32_t>(b1) << 24) |
                        (static_cast<std::uint32_t>(b2) << 16) |
                        (static_cast<std::uint32_t>(b3) << 8)  | b4;
            if (endian == enEndian::littleEndian) ret = valueConverter::swapUInt32(ret);
            value.push_back(ret);
        }
    }else{  // If the number of elements is 2 or greater, retrieve the value at the location indicated by the Offset value
        std::uint32_t ofs = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
                (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
                (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
        if (endian == enEndian::littleEndian) ofs = valueConverter::swapUInt32(ofs);
        if (ofs + iCount * 4 > data.size()) return false;
        for (int i = 0; i < iCount; i++){
            std::uint8_t b1 = static_cast<std::uint8_t>(data[ofs + i * 4]);
            std::uint8_t b2 = static_cast<std::uint8_t>(data[ofs + i * 4 + 1]);
            std::uint8_t b3 = static_cast<std::uint8_t>(data[ofs + i * 4 + 2]);
            std::uint8_t b4 = static_cast<std::uint8_t>(data[ofs + i * 4 + 3]);
            std::uint32_t ret = (static_cast<std::uint32_t>(b1) << 24) |
                        (static_cast<std::uint32_t>(b2) << 16) |
                        (static_cast<std::uint32_t>(b3) << 8)  | b4;
            if (endian == enEndian::littleEndian) ret = valueConverter::swapUInt32(ret);
            value.push_back(ret);
        }
    }
    //
    return true;
};

bool IFDReader::getIFDValue(const std::uint32_t offset, stIFDTypeValue &value){
    const std::uint16_t APP1HeaderSize = sizeof(stAPP1::TIFFByteOrder) + sizeof(stAPP1::TIFFIdentifier) +
            sizeof(stAPP1::IFD0thOffset);
    std::uint32_t iOffset = offset + APP1HeaderSize;
    //
    bool found = false;
    for (stIFDTag ifd: IFDTable){
        if (ifd.type == static_cast<std::uint16_t>(enIFDType::BYTE)){
            std::vector<std::uint8_t> retVal;
            bool res = getIFDValueByte(ifd.tag, iOffset, enendian, ardata, retVal);
            if (!res) continue;
            value.byteValues = retVal;
            found = true;
            continue;
        }
        if (ifd.type == static_cast<std::uint16_t>(enIFDType::ASCII)){
            std::string retVal;
            bool res = getIFDValueAscii(ifd.tag, iOffset, enendian, ardata, retVal);
            if (!res) continue;
            value.asciiValue = retVal;
            found = true;
            continue;
        }
    }
    //
    return found;
}

