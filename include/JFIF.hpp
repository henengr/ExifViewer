#pragma once

#include <cstdint>


enum enSegmentMarker: std::uint16_t {
    SOI         = 0xffd8,
    APP0        = 0xffe0,
    APP1        = 0xffe1,
    APP2        = 0xffe2,
    APP3        = 0xffe3,
    APP4        = 0xffe4,
    APP5        = 0xffe5,
    APP6        = 0xffe6,
    APP7        = 0xffe7,
    APP8        = 0xffe8,
    APP9        = 0xffe9,
    APP10       = 0xffea,
    APP11       = 0xffeb,
    APP12       = 0xffec,
    APP13       = 0xffed,
    APP14       = 0xffee,
    APP15       = 0xffef,
    DQT         = 0xffdb,
    SOF0        = 0xffc0,
    DHT         = 0xffc4,
    SOS         = 0xffda,
    EOI         = 0xffd9,
    JPG0        = 0xfff0,
    JPG1        = 0xfff1,
    JPG2        = 0xfff2,
    JPG3        = 0xfff3,
    JPG4        = 0xfff4,
    JPG5        = 0xfff5,
    JPG6        = 0xfff6,
    JPG7        = 0xfff7,
    JPG8        = 0xfff8,
    JPG9        = 0xfff9,
    JPG10       = 0xfffa,
    JPG11       = 0xfffb,
    JPG12       = 0xfffc,
    JPG13       = 0xfffd,
    COM         = 0xfffe,
};

struct stSOI {
    std::uint16_t       marker;                     // 0000
};

struct stAPP0 {
    std::uint16_t       marker;                     // 0000
    std::uint16_t       length;                     // 0002
    std::uint8_t        JFIFIdentifier[5];          // 0004  "JFIF\0"
    std::uint16_t       version;                    // 0009
    std::uint8_t        pixelConcentrationUnit;     // 000b
    std::uint16_t       pixelConcentrationX;        // 000c
    std::uint16_t       pixelConcentrationY;        // 000e
    //
    std::uint8_t        thumbnailImageWidth;        // 0010
    std::uint8_t        thumbnailImageHeight;       // 0011
    std::uint8_t        thumbnailImage[1];          // 0012
};

struct stAPP1 {
    std::uint16_t       marker;                     // 0000
    std::uint16_t       length;                     // 0002
    std::uint8_t        ExifIdentifier[6];          // 0004  "Exif\0\0"
    std::uint8_t        TIFFByteOrder[2];           // 000a
    std::uint8_t        TIFFIdentifier;             // 000b  0x2a(42)
    std::uint8_t        IFDs[1];                    // 000c
};

struct stDQT {
    std::uint16_t       marker;                     // 0000
    std::uint16_t       length;                     // 0002
    std::uint8_t        tableIdentifier;            // 0004
    std::uint8_t        DQTtable[1];                // 0005
};

struct stSOF0 {
    std::uint16_t       marker;                     // 0000
    std::uint16_t       length;                     // 0002
    std::uint8_t        samplePrecision;            // 0004
    std::uint16_t       imageRow;                   // 0005
    std::uint16_t       imageColumn;                // 0007
    std::uint8_t        numOfComponents;            // 0009
    std::uint8_t        components[1];              // 000a
};

struct stDHT {
    std::uint16_t       marker;                     // 0000
    std::uint16_t       length;                     // 0002
    std::uint8_t        DHTClassIdentifier;         // 0004
    std::uint8_t        numOfCodesByCodeLength[16]; // 0005
    std::uint8_t        codesOfEachCodeLength[1];   // 0015
};

struct stSOS {
    std::uint16_t       marker;                     // 0000
    std::uint16_t       length;                     // 0002
    std::uint8_t        numOfComponents;            // 0004
    std::uint8_t        components[1];              // 0005
};

struct stEOI {
    std::uint16_t       marker;                     // 0000
};

