#pragma once

#include <cstdint>
#include <vector>


enum enIFDTag: std::uint16_t {
    imageWidth          = 0x0100,
    imageLength         = 0x0101,
    bitsPerSample       = 0x0102,
    compression         = 0x0103,
    photometricInterpretation = 0x0106,
    orientation         = 0x0112,
    samplesPerPixel     = 0x0115,
    plannerConfiguration = 0x011c,
    YCbCrSubSampling    = 0x0212,
    YCbCrPositioning    = 0x0213,
    XResolution         = 0x011a,
    YResolution         = 0x011b,
    resolutionUnit      = 0x0128,
    //
    stripOffsets        = 0x0111,
    rowsPerStrip        = 0x0116,
    stripByteCounts     = 0x0117,
    JPEGInterchangeFormat = 0x0201,
    JPEGInterchangeFormatLength = 0x0202,
    //
    transferFanction    = 0x012d,
    whitePoint          = 0x013e,
    primaryChromaticities = 0x013f,
    YCbCrCoefficients   = 0x0211,
    referenceBlackWhite = 0x0214,
    //
    dateTime            = 0x0132,
    imageDescription    = 0x010e,
    make                = 0x010f,
    model               = 0x0110,
    software            = 0x0131,
    artist              = 0x013b,
    copyright           = 0x8298,
    //

};

enum enIFDType: std::uint16_t {
    BYTE = 1,           // 8-bit unsigned integer
    ASCII,              // ASCII character
    SHORT,              // 16-bit unsigned integer
    LONG,               // 32-bit unsigned integer
    RATIONAL,           // 2 of 16-bit unsigned integer - The first value represents the numerator, and the second value represents the denominator.
    UNDEFINED = 7,      // undedined 8-bit value
    SLONG = 9,          // 32-bit signed integer
    SRATIONAL,          // 2 of 16-bit signed integer - The first value represents the numerator, and the second value represents the denominator.
};


class ExifIFDTool {
public:
};
