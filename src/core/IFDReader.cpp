#include "IFDReader.hpp"
#include "JPEGReader.hpp"


std::int32_t IFDReader::readIFD(const std::uint32_t offset, const enEndian endian, const std::vector<std::uint8_t> &data,
                stIFD &ifd){
    std::uint32_t offs = offset;
    if (offs + 12 > data.size()) return IFDREADER_INVALID_IFD_SIZE;
    //
    ifd.tag = (static_cast<std::uint16_t>(data[offs]) << 8) | data[offs + 1];
    offs += sizeof(ifd.tag);
    ifd.type = (static_cast<std::uint16_t>(data[offs]) << 8) | data[offs + 1];
    offs += sizeof(ifd.type);
    ifd.count = (static_cast<std::uint32_t>(data[offs]) << 24) | 
            (static_cast<std::uint32_t>(data[offs + 1]) << 16) |
            (static_cast<std::uint32_t>(data[offs + 2]) << 8) | data[offs + 3];
    offs += sizeof(ifd.count);
    std::uint32_t ofs = (static_cast<std::uint32_t>(data[offs]) << 24) | 
            (static_cast<std::uint32_t>(data[offs + 1]) << 16) |
            (static_cast<std::uint32_t>(data[offs + 2]) << 8) | data[offs + 3];
    if (endian == enEndian::littleEndian){
        ifd.tag = valueConverter::swapUInt16(ifd.tag);
        ifd.type = valueConverter::swapUInt16(ifd.type);
        ifd.count = valueConverter::swapUInt32(ifd.count);
        ofs = valueConverter::swapUInt32(ofs);
    }
    //
    ifd.data.clear();
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::BYTE) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::ASCII) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::UNDEFINED) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::UTF8)){
        if (ifd.count <= 4){
            auto is = data.begin() + offs;
            auto ie = is + ifd.count;
            ifd.data.assign(is, ie);
        } else {
            if (ofs + ifd.count > data.size()) return IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE;
            auto is = data.begin() + ofs;
            auto ie = is + ifd.count;
            ifd.data.assign(is, ie);
        }
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::SHORT)){
        if (ifd.count <= 2){
            auto is = data.begin() + offs;
            auto ie = is + ifd.count * 2;
            ifd.data.assign(is, ie);
        } else {
            if (ofs + ifd.count * 2 > data.size()) return IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE;
            auto is = data.begin() + ofs;
            auto ie = is + ifd.count * 2;
            ifd.data.assign(is, ie);
        }
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::LONG) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::SLONG)){
        if (ifd.count <= 1){
            auto is = data.begin() + offs;
            auto ie = is + ifd.count * 4;
            ifd.data.assign(is, ie);
        } else {
            if (ofs + ifd.count * 4 > data.size()) return IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE;
            auto is = data.begin() + ofs;
            auto ie = is + ifd.count * 4;
            ifd.data.assign(is, ie);
        }
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::RATIONAL) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::SRATIONAL)){
        if (ofs + ifd.count * 8 > data.size()) return IFDREADER_INVALID_VALUE_LONGER_THAN_FOURBYTE_SIZE;
        auto is = data.begin() + ofs;
        auto ie = is + ifd.count * 8;
        ifd.data.assign(is, ie);
    }
    //
    return 12;
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
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::BYTE) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::UNDEFINED)){
        value.values = ifd.data;
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::ASCII) ||
        ifd.type == static_cast<std::uint16_t>(enIFDType::UTF8)){
        std::string str = "";
        for (std::uint32_t i = 0; i < ifd.count; i++){
            str += static_cast<char>(ifd.data[i]);
        }
        if (!str.empty() && str.back() == '\0') str.pop_back();
        value.values = str;
    }
    if (ifd.type == static_cast<std::uint16_t>(enIFDType::SHORT)){
        std::vector<std::uint16_t> vals;
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
            std::uint32_t l1 = (static_cast<std::uint32_t>(ifd.data[i]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 1]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 2]) << 8) | ifd.data[i + 3];
            std::uint32_t l2 = (static_cast<std::uint32_t>(ifd.data[i + 4]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 5]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 6]) << 8) | ifd.data[i + 7];
            if (endian == enEndian::littleEndian) {
                l1 = valueConverter::swapUInt32(l1);
                l2 = valueConverter::swapUInt32(l2);
            }
            vals.push_back(stRational{l1, l2});
        }
        value.values = vals;
    }
    else if (ifd.type == static_cast<std::uint16_t>(enIFDType::SRATIONAL)){
        std::vector<stSRational> vals;
        for (std::uint32_t i = 0; i + 7 < ifd.data.size(); i += 8) {
            std::uint32_t l1 = (static_cast<std::uint32_t>(ifd.data[i]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 1]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 2]) << 8) | ifd.data[i + 3];
            std::uint32_t l2 = (static_cast<std::uint32_t>(ifd.data[i + 4]) << 24) |
                                (static_cast<std::uint32_t>(ifd.data[i + 5]) << 16) |
                                (static_cast<std::uint32_t>(ifd.data[i + 6]) << 8) | ifd.data[i + 7];
            if (endian == enEndian::littleEndian) {
                l1 = valueConverter::swapUInt32(l1);
                l2 = valueConverter::swapUInt32(l2);
            }
            vals.push_back(stSRational{static_cast<std::int32_t>(l1), static_cast<std::int32_t>(l2)});
        }
        value.values = vals;
    }
    return true;
}

std::int32_t IFDReader::readIFDs(const std::uint32_t offset, const std::vector<std::uint8_t> &data, std::vector<stIFD> &ifds){
    std::uint32_t iOffset = offset;
    std::uint32_t minSize = 10;     // the minimam size
    if (iOffset + minSize > data.size()) return IFDREADER_INVALID_IFD_SIZE;
    // ByteOrder
    char endianChrs[2] = {
        static_cast<char>(data.at(iOffset)), 
        static_cast<char>(data.at(iOffset + 1))
    };
    enEndian endian;
    if (endianChrs[0] == 'M' && endianChrs[1] == 'M')
        endian = enEndian::bigEndian;
    else if (endianChrs[0] == 'I' && endianChrs[1] == 'I')
        endian = enEndian::littleEndian;
    else return IFDREADER_INVALID_ENDIAN_SETTING;
    iOffset += sizeof(endianChrs);
    // Header 42
    std::uint16_t header42 = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    if (endian == enEndian::littleEndian) header42 = valueConverter::swapUInt16(header42);
    if (header42 != 0x002a) return IFDREADER_INVALID_HEADER42_VALUE;
    iOffset += sizeof(header42);
    // 0th IFD Offset
    std::uint32_t ofs0thIFD = (static_cast<std::uint32_t>(data[iOffset]) << 24) |
                        (static_cast<std::uint32_t>(data[iOffset + 1]) << 16) |
                        (static_cast<std::uint32_t>(data[iOffset + 2]) << 8) | data[iOffset + 3];
    if (endian == enEndian::littleEndian) ofs0thIFD = valueConverter::swapUInt32(ofs0thIFD);
    if (iOffset + ofs0thIFD > data.size()) return IFDREADER_INVALID_NUM_OF_IFDS;
    iOffset = ofs0thIFD + offset;
    // Number of Entries
    std::uint16_t numOfEntries = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    if (endian == enEndian::littleEndian) numOfEntries = valueConverter::swapUInt16(numOfEntries);
    if (iOffset + numOfEntries * 12 > data.size()) return IFDREADER_INVALID_NUM_OF_IFDS;
    iOffset += sizeof(numOfEntries);
    // 0th IFD
    std::uint32_t exifIFDPointer = 0;
    std::uint32_t GPSIFDOffset = 0;
    ifds.clear();
    for (std::uint16_t i = 0; i < numOfEntries; i++){
        stIFD ifd;
        bool found = false;

        readIFD(iOffset, endian, data, ifd);
        for (stIFDTag tag: IFDTable){
            if (ifd.tag == tag.tag){
                ifd.description = tag.description;
                ifd.offset = offset;
                found = true;
                break;
            }
        }
        if (!found){
            ifd.description = IFD_UNKNOWN_TAG;
        }

        ifds.push_back(ifd);
        if (ifd.tag == IFDTAG_ExifIFDPointer){
            stIFDValue val;
            getVariantIFD(ifd, endian, val);
            std::vector<std::uint32_t> vals = std::get<std::vector<std::uint32_t>>(val.values);
            if (vals.size() >= 1) exifIFDPointer = vals[0];
        }
        if (ifd.tag == IFDTAG_GPSInfoIFDPointer){
            stIFDValue val;
            getVariantIFD(ifd, endian, val);
            std::vector<std::uint32_t> vals = std::get<std::vector<std::uint32_t>>(val.values);
            if (vals.size() >= 1) GPSIFDOffset = vals[0];
        }
        iOffset += 12;
    }
    if (exifIFDPointer == 0) return iOffset;
    iOffset = exifIFDPointer + offset;
    // Exif IFD
    std::uint16_t ExifIFDNumber = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    if (endian == enEndian::littleEndian) ExifIFDNumber = valueConverter::swapUInt16(ExifIFDNumber);
    iOffset += sizeof(ExifIFDNumber);
    if (iOffset + ExifIFDNumber * 12 > data.size()) return IFDREADER_INVALID_NUM_OF_IFDS;
    //
    for (std::uint16_t i = 0; i < ExifIFDNumber; i++){
        stIFD ifd;
        bool found = false;

        readIFD(iOffset, endian, data, ifd);
        for (stIFDTag tag: IFDTable){
            if (ifd.tag == tag.tag){
                ifd.description = tag.description;
                ifd.offset = offset;
                found = true;
                break;
            }
        }
        if (!found){
            ifd.description = IFD_UNKNOWN_TAG;
        }

        ifds.push_back(ifd);
        iOffset += 12;
    }
    // 1st IFD
    if (GPSIFDOffset == 0) return iOffset;
    iOffset = GPSIFDOffset + offset;
    numOfEntries = (static_cast<std::uint16_t>(data[iOffset]) << 8) | data[iOffset + 1];
    if (endian == enEndian::littleEndian) numOfEntries = valueConverter::swapUInt16(numOfEntries);
    iOffset += sizeof(numOfEntries);
    if (iOffset + numOfEntries * 12 > data.size()) return IFDREADER_INVALID_NUM_OF_IFDS;
    //
    for (std::uint16_t i = 0; i < numOfEntries; i++){
        stIFD ifd;
        bool found = false;

        readIFD(iOffset, endian, data, ifd);
        for (stIFDTag tag: IFDTable){
            if (ifd.tag == tag.tag){
                ifd.description = tag.description;
                ifd.offset = offset;
                found = true;
                break;
            }
        }
        if (!found){
            ifd.description = IFD_UNKNOWN_TAG;
        }

        ifds.push_back(ifd);
        iOffset += 12;
    }
    //
    return iOffset;
}

std::int32_t IFDReader::readIFDs(std::vector<stIFD> &ifds){
    return readIFDs(0, _data, ifds);
}

