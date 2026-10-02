#include "IFDReader.hpp"
#include "JPEGReader.hpp"


bool IFDReader::getIFDValueByte(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, stIFDTypeValue &value){
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
    value.type = enIFDType::BYTE;
    value.byteValues.clear();  // initialize the vector
    if (iCount <= 4){   // If the number of elements is 4 or less, retrieve the value from the "Offset" value
        for (int i = 0; i < iCount; i++){
            value.byteValues.push_back(data[iOffset + i]);
        }
    }else{  // If the number of elements is 5 or greater, retrieve the value at the location indicated by the Offset value
        std::uint32_t ofs = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
                (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
                (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
        if (endian == enEndian::littleEndian) ofs = valueConverter::swapUInt32(ofs);
        if (ofs + iCount > data.size()) return false;
        for (int i = 0; i < iCount; i++){
            value.byteValues.push_back(data[ofs + i]);
        }
    }
    //
    return true;
};

bool IFDReader::getIFDValueAscii(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, stIFDTypeValue &value){
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
    value.type = enIFDType::ASCII;
    value.asciiValue.clear();  // initialize the string
    if (iCount <= 4){   // If the number of elements is 4 or less, retrieve the value from the "Offset" value
        for (int i = 0; i < iCount; i++){
            char c = static_cast<char>(data[iOffset + i]);
            if (c == '\0') break;
            value.asciiValue += c;
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
            value.asciiValue += c;
        }
    }
    //
    return true;
};

bool IFDReader::getIFDValueShort(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, stIFDTypeValue &value){
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
    value.type = enIFDType::SHORT;
    value.shortValues.clear();  // initialize the vector
    if (iCount <= 2){   // If the number of elements is 2 or less, retrieve the value from the "Offset" value
        for (int i = 0; i < iCount; i++){
            std::uint8_t b1 = static_cast<std::uint8_t>(data[iOffset + i * 2]);
            std::uint8_t b2 = static_cast<std::uint8_t>(data[iOffset + i * 2 + 1]);
            std::uint16_t ret = (static_cast<std::uint16_t>(b1) << 8) | b2;
            if (endian == enEndian::littleEndian) ret = valueConverter::swapUInt16(ret);
            value.shortValues.push_back(ret);
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
            value.shortValues.push_back(ret);
        }
    }
    //
    return true;
};

bool IFDReader::getIFDValueLong(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, stIFDTypeValue &value){
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
    value.type = enIFDType::LONG;
    value.longValues.clear();  // initialize the vector
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
            value.longValues.push_back(ret);
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
            value.longValues.push_back(ret);
        }
    }
    //
    return true;
};

bool IFDReader::getIFDValueRational(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, stIFDTypeValue &value){
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
    if (iType != static_cast<std::uint16_t>(enIFDType::RATIONAL)) return false;
    if (iOffset + iCount * 8 > data.size()) return false;
    //
    value.type = enIFDType::RATIONAL;
    value.rationalValues.clear();  // initialize the vector
    std::uint32_t ofs = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
            (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
            (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
    if (endian == enEndian::littleEndian) ofs = valueConverter::swapUInt32(ofs);
    if (ofs + iCount * 8 > data.size()) return false;
    for (int i = 0; i < iCount; i++){
        std::uint8_t b1 = static_cast<std::uint8_t>(data[ofs + i * 4]);
        std::uint8_t b2 = static_cast<std::uint8_t>(data[ofs + i * 4 + 1]);
        std::uint8_t b3 = static_cast<std::uint8_t>(data[ofs + i * 4 + 2]);
        std::uint8_t b4 = static_cast<std::uint8_t>(data[ofs + i * 4 + 3]);
        std::uint8_t b5 = static_cast<std::uint8_t>(data[ofs + i * 4 + 4]);
        std::uint8_t b6 = static_cast<std::uint8_t>(data[ofs + i * 4 + 5]);
        std::uint8_t b7 = static_cast<std::uint8_t>(data[ofs + i * 4 + 6]);
        std::uint8_t b8 = static_cast<std::uint8_t>(data[ofs + i * 4 + 7]);
        std::uint32_t ret1 = (static_cast<std::uint32_t>(b1) << 24) |
                    (static_cast<std::uint32_t>(b2) << 16) |
                    (static_cast<std::uint32_t>(b3) << 8)  | b4;
        std::uint32_t ret2 = (static_cast<std::uint32_t>(b5) << 24) |
                    (static_cast<std::uint32_t>(b6) << 16) |
                    (static_cast<std::uint32_t>(b7) << 8)  | b8;
        if (endian == enEndian::littleEndian){
            ret1 = valueConverter::swapUInt32(ret1);
            ret2 = valueConverter::swapUInt32(ret2);
        }
        stRational ratValue = {ret1, ret2};
        value.rationalValues.push_back(ratValue);
    }
    //
    return true;
};

bool IFDReader::getIFDValueUndefined(const std::uint16_t tag, const std::uint32_t offset, const enEndian endian,
        const std::vector<std::uint8_t> &data, stIFDTypeValue &value){
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
    if (iType != static_cast<std::uint16_t>(enIFDType::UNDEFINED)) return false;
    if (iOffset + iCount > data.size()) return false;
    //
    value.type = enIFDType::UNDEFINED;
    value.undefinedValue.clear();  // initialize the vector
    if (iCount <= 4){   // If the number of elements is 4 or less, retrieve the value from the "Offset" value
        for (int i = 0; i < iCount; i++){
            value.undefinedValue.push_back(data[iOffset + i]);
        }
    }else{  // If the number of elements is 5 or greater, retrieve the value at the location indicated by the Offset value
        std::uint32_t ofs = (static_cast<std::uint32_t>(data[iOffset]) << 24) | 
                (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
                (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
        if (endian == enEndian::littleEndian) ofs = valueConverter::swapUInt32(ofs);
        if (ofs + iCount > data.size()) return false;
        for (int i = 0; i < iCount; i++){
            value.undefinedValue.push_back(data[ofs + i]);
        }
    }
    //
    return true;
};

bool IFDReader::getIFDValue(const std::uint32_t offset, const enEndian endian, std::vector<std::uint8_t> &data, 
        stIFDTypeValue &value){
    std::uint32_t iOffset = offset;
    bool found = false;
    for (stIFDTag ifd: IFDTable){
        if (found) break;
        if (ifd.type == static_cast<std::uint16_t>(enIFDType::BYTE)){
            bool res = getIFDValueByte(ifd.tag, iOffset, endian, data, value);
            if (!res) continue;
            value.tag = ifd;
            found = true;
            continue;
        }
        if (ifd.type == static_cast<std::uint16_t>(enIFDType::ASCII)){
            bool res = getIFDValueAscii(ifd.tag, iOffset, endian, data, value);
            if (!res) continue;
            value.tag = ifd;
            found = true;
            continue;
        }
        if (ifd.type == static_cast<std::uint16_t>(enIFDType::SHORT)){
            bool res = getIFDValueShort(ifd.tag, iOffset, endian, data, value);
            if (!res) continue;
            value.tag = ifd;
            found = true;
            continue;
        }
        if (ifd.type == static_cast<std::uint16_t>(enIFDType::LONG)){
            bool res = getIFDValueLong(ifd.tag, iOffset, endian, data, value);
            if (!res) continue;
            value.tag = ifd;
            found = true;
            continue;
        }
        if (ifd.type == static_cast<std::uint16_t>(enIFDType::RATIONAL)){
            bool res = getIFDValueRational(ifd.tag, iOffset, endian, data, value);
            if (!res) continue;
            value.tag = ifd;
            found = true;
            continue;
        }
        if (ifd.type == static_cast<std::uint16_t>(enIFDType::UNDEFINED)){
            bool res = getIFDValueUndefined(ifd.tag, iOffset, endian, data, value);
            if (!res) continue;
            value.tag = ifd;
            found = true;
            continue;
        }
    }
    return found;
}


bool IFDReader::getIFDValues(const std::uint32_t offset, std::vector<stIFDTypeValue> &values){
    const std::uint16_t APP1HeaderSize = sizeof(stAPP1::TIFFByteOrder) + sizeof(stAPP1::TIFFIdentifier) +
            sizeof(stAPP1::IFD0thOffset);
    std::uint32_t iOffset = offset + APP1HeaderSize;
    if (iOffset > ardata.size()) return false;
    //
    std::uint16_t numOfEntries = (static_cast<std::uint16_t>(ardata[iOffset]) << 8) | ardata[iOffset + 1];
    if (enendian == enEndian::littleEndian) numOfEntries = valueConverter::swapUInt16(numOfEntries);
    iOffset += sizeof(numOfEntries);
    //
    stIFDTypeValue value;
    std::uint32_t iExifIFDPtr;
    std::uint32_t iGPSIFDPtr;
    stIFDTag ifd;
    for (std::uint16_t i = 0; i < numOfEntries; i++){
        bool res = getIFDValue(iOffset, enendian, ardata, value);
        if (!res) continue;
        values.push_back(value);
        if (ifd.tag == 0x8769 && ifd.type == to_u16(enIFDType::LONG) && value.longValues.size() > 0)
                iExifIFDPtr = value.longValues[0];      // Exif IFD Pointer
        if (ifd.tag == 0x8825 && ifd.type == to_u16(enIFDType::LONG) && value.longValues.size() > 0)
                iGPSIFDPtr = value.longValues[0];       // GPS IFD Pointer
        // move to next IFD
        iOffset += sizeof(std::uint16_t) + sizeof(std::uint16_t) + sizeof(std::uint32_t) + sizeof(std::uint32_t);
    }

    //
    return true;
}

