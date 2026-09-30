#pragma once

#include <cstdint>
#include <vector>
#include <string>


enum class enIFDType: std::uint16_t {
    BYTE = 1,           // 8-bit unsigned integer
    ASCII,              // ASCII character
    SHORT,              // 16-bit unsigned integer
    LONG,               // 32-bit unsigned integer
    RATIONAL,           // 2 of 16-bit unsigned integer - The first value represents the numerator, and the second value represents the denominator.
    UNDEFINED = 7,      // undedined 8-bit value
    SLONG = 9,          // 32-bit signed integer
    SRATIONAL,          // 2 of 16-bit signed integer - The first value represents the numerator, and the second value represents the denominator.
};

struct stIFDTag {
    std::uint16_t       tag;
    std::uint16_t       type;
    std::uint32_t       count;
    std::uint32_t       offset;
};

struct stIFDTypeValue {
    enIFDType                   type;
    std::vector<std::uint8_t>   byteValues;
    std::string                 asciiValue;
    std::vector<std::uint16_t>  shortValues;
    std::vector<std::uint32_t>  longValues;
    std::vector<std::uint8_t>   undefinedValue;
    std::vector<std::int32_t>   slongValues;
};

constexpr std::uint16_t to_u16(enIFDType t){
    return static_cast<std::uint16_t>(t);
}

//const std::vector<stIFDTag> IFDTable = {
constexpr stIFDTag IFDTable[] = {
    // Tags related to image composition
    {0x8769, to_u16(enIFDType::LONG),               1,  0},                 // Exif IFD Pointer
    {0x8825, to_u16(enIFDType::LONG),               1,  0},                 // GPS Info IFD Pointer
    {0xa005, to_u16(enIFDType::LONG),               1,  0},                 // Interoperability IFD Pointer
    // TIFF tags used in Exif
    {0x0100, to_u16(enIFDType::SHORT) | to_u16(enIFDType::LONG),  1,  0},   // ImageWidth
    {0x0101, to_u16(enIFDType::SHORT) | to_u16(enIFDType::LONG),  1,  0},   // ImageHeight
    {0x0102, to_u16(enIFDType::SHORT),              3,  0},                 // BitsPerSample
    {0x0103, to_u16(enIFDType::SHORT),              1,  0},                 // Compression
    {0x0106, to_u16(enIFDType::SHORT),              1,  0},                 // PhotometricInterpretation
    {0x0112, to_u16(enIFDType::SHORT),              1,  0},                 // Orientation
    {0x0115, to_u16(enIFDType::SHORT),              1,  0},                 // SamplesPerPixel
    {0x011c, to_u16(enIFDType::SHORT),              1,  0},                 // PlannerConfiguration
    {0x0212, to_u16(enIFDType::SHORT),              2,  0},                 // YCbCrSubSampling
    {0x0213, to_u16(enIFDType::SHORT),              1,  0},                 // YCbCrPositioning
    {0x011a, to_u16(enIFDType::RATIONAL),           1,  0},                 // XResolution
    {0x011b, to_u16(enIFDType::RATIONAL),           1,  0},                 // YResolution
    {0x0128, to_u16(enIFDType::SHORT),              1,  0},                 // ResolutionUnit
    {0x0111, to_u16(enIFDType::SHORT) | to_u16(enIFDType::LONG),  1,  0},   // StripOffsets
    {0x0116, to_u16(enIFDType::SHORT) | to_u16(enIFDType::LONG),  1,  0},   // RowsPerStrip
    {0x0117, to_u16(enIFDType::SHORT) | to_u16(enIFDType::LONG),  1,  0},   // StripByteCounts
    {0x0201, to_u16(enIFDType::LONG),               1,  0},                 // JPEGInterchangeFormat
    {0x0202, to_u16(enIFDType::LONG),               1,  0},                 // JPEGInterchangeFormatLength
    {0x012d, to_u16(enIFDType::SHORT),        3 * 256,  0},                 // TransferFunction
    {0x013e, to_u16(enIFDType::RATIONAL),           2,  0},                 // WhitePoint
    {0x013f, to_u16(enIFDType::RATIONAL),           6,  0},                 // PrimaryChromaticities
    {0x0211, to_u16(enIFDType::RATIONAL),           3,  0},                 // YCbCrCoefficients
    {0x0214, to_u16(enIFDType::RATIONAL),           6,  0},                 // ReferenceBlackWhite
    {0x0132, to_u16(enIFDType::ASCII),             20,  0},                 // DateTime
    {0x010e, to_u16(enIFDType::ASCII),         0xffff,  0},                 // ImageDescription
};

enum enIFDTag: std::uint16_t {
    rowsPerStrip        = 0x0116,
    stripByteCounts     = 0x0117,
    JPEGInterchangeFormat = 0x0201,
    JPEGInterchangeFormatLength = 0x0202,
    transferFanction    = 0x012d,
    whitePoint          = 0x013e,
    primaryChromaticities = 0x013f,
    YCbCrCoefficients   = 0x0211,
    referenceBlackWhite = 0x0214,
    dateTime            = 0x0132,
    imageDescription    = 0x010e,
    make                = 0x010f,
    model               = 0x0110,
    software            = 0x0131,
    artist              = 0x013b,
    copyright           = 0x8298,
    // Exif IFDs
    ExifVersion         = 0x9000,
    FlashpixVersion     = 0xa000,
    colorSpace          = 0xa001,
    gamma               = 0xa500,
    componentsConfiguration = 0x9101,
    compressedBitsPerPixel = 0x9102,
    pixelXDimension     = 0xa002,
    pixelYDimension     = 0xa003,
    makerNote           = 0x927c,
    userComment         = 0x9286,

};


