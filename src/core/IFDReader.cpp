#include "IFDReader.hpp"
#include "JPEGReader.hpp"


std::int32_t IFDReader::readIFD(const std::uint32_t offset, const enEndian endian, const std::vector<std::uint8_t> &data,
                stIFD &ifd){
    std::uint32_t iOffset = offset;
    if (iOffset + 12 > data.size()) return IFDREADER_INVALID_IFD_SIZE;
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
    ifd.data.clear();
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::BYTE) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::ASCII) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::UNDEFINED)){
        if (ifd.count <= 4){
            auto is = data.begin() + iOffset;
            auto ie = is + ifd.count;
            ifd.data.assign(is, ie);
        } else {
            if (iOfs + ifd.count > data.size()) return IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE;
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
            if (iOfs + ifd.count * 2 > data.size()) return IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE;
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
            if (iOfs + ifd.count * 4 > data.size()) return IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE;
            auto is = data.begin() + iOfs;
            auto ie = is + ifd.count * 4;
            ifd.data.assign(is, ie);
        }
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::RATIONAL) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::SRATIONAL)){
        if (iOfs + ifd.count * 8 > data.size()) return IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE;
        auto is = data.begin() + iOfs;
        auto ie = is + ifd.count * 8;
        ifd.data.assign(is, ie);
    }
    //
    iOffset += sizeof(iOfs);
    return static_cast<std::int32_t>(iOffset);
}

bool IFDReader::getVariantIFD(const stIFD &ifd, const enEndian endian, stIFDValue &value){
    value.IFD.tag = ifd.tag;
    value.IFD.type = ifd.type;
    value.IFD.count = ifd.count;
    value.IFD.offset = 0;
    bool found = false;
    for (stIFDTag ifdt: IFDTable){
        if (ifdt.tag == ifd.tag){
            value.IFD.description = ifdt.description;
            found = true;
            break;
        }
    }
    if (!found){
        value.IFD.description = IFD_UNKNOWN_TAG;
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::BYTE) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::UNDEFINED)){
        value.values = ifd.data;
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::ASCII)){
        std::string str = "";
        for (std::uint32_t i = 0; i < ifd.count; i++){
            str += static_cast<char>(ifd.data[i]);
        }
        if (!str.empty() && str.back() == '\0') str.pop_back();
        value.values = str;
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::SHORT)){
        std::vector<std::uint16_t> vals;
        //for (std::uint32_t i = 0; i < ifd.count; i++){
        for (std::uint32_t i = 0; i + 1< ifd.data.size(); i += 2){
            std::uint16_t val = (static_cast<std::uint16_t>(ifd.data[i]) << 8) | ifd.data[i + 1];
            if (endian == enEndian::littleEndian) val = valueConverter::swapUInt16(val);
            vals.push_back(val);
        }
        value.values = vals;
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::LONG)){
        std::vector<std::uint32_t> vals;
        for (std::uint32_t i = 0; i + 3 < ifd.data.size(); i += 4) {
            std::uint32_t val = (static_cast<std::uint32_t>(ifd.data[i]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 1]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 2]) << 8) | ifd.data[i + 3];
            if (endian == enEndian::littleEndian) val = valueConverter::swapUInt32(val);
            vals.push_back(val);
        }
        value.values = vals;
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::SLONG)){
        std::vector<std::int32_t> vals;
        for (std::uint32_t i = 0; i + 3 < ifd.data.size(); i += 4) {
            std::uint32_t val = (static_cast<std::uint32_t>(ifd.data[i]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 1]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 2]) << 8) | ifd.data[i + 3];
            if (endian == enEndian::littleEndian) val = valueConverter::swapUInt32(val);
            vals.push_back(static_cast<std::int32_t>(val));
        }
        value.values = vals;
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::RATIONAL)){
        std::vector<stRational> vals;
        for (std::uint32_t i = 0; i + 7 < ifd.data.size(); i += 8) {
            std::uint32_t num = (static_cast<std::uint32_t>(ifd.data[i]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 1]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 2]) << 8) | ifd.data[i + 3];
            std::uint32_t den = (static_cast<std::uint32_t>(ifd.data[i + 4]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 5]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 6]) << 8) | ifd.data[i + 7];
            if (endian == enEndian::littleEndian) {
                num = valueConverter::swapUInt32(num);
                den = valueConverter::swapUInt32(den);
            }
            vals.push_back(stRational{num, den});
        }
        value.values = vals;
    }
    else if (ifd.type == static_cast<std::uint16_t>(enIFDType::RATIONAL)){
        std::vector<stSRational> vals;
        for (std::uint32_t i = 0; i + 7 < ifd.data.size(); i += 8) {
            std::uint32_t num = (static_cast<std::uint32_t>(ifd.data[i]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 1]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 2]) << 8) | ifd.data[i + 3];
            std::uint32_t den = (static_cast<std::uint32_t>(ifd.data[i + 4]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 5]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 6]) << 8) | ifd.data[i + 7];
            if (endian == enEndian::littleEndian) {
                num = valueConverter::swapUInt32(num);
                den = valueConverter::swapUInt32(den);
            }
            vals.push_back(stSRational{static_cast<std::int32_t>(num), static_cast<std::int32_t>(den)});
        }
        value.values = vals;
    }
    return true;
}