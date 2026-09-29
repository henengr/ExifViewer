#pragma once

#include <cstdint>
#include <vector>


enum enSegmentMarker: std::uint16_t {
    TEM         = 0xff01,
    SOC         = 0xff4f,
	SIZ         = 0xff51,
    COD         = 0xff52,
    COC         = 0xff53,
    TLM         = 0xff55,
    PLM         = 0xff57,
    PLT         = 0xff58,
    QCD         = 0xff5c,
    QCC         = 0xff5d,
    RGN         = 0xff5e,
    POC         = 0xff5f,
    PPM         = 0xff60,
    PPT         = 0xff61,
    CRG         = 0xff63,
    COM         = 0xff64,
    SOT         = 0xff90,
    SOP         = 0xff91,
    EPH         = 0xff92,
    SOD         = 0xff93,
    SOF0        = 0xffc0,
    SOF1        = 0xffc1,
    SOF2        = 0xffc2,
    SOF3        = 0xffc3,
    DHT         = 0xffc4,
    SOF5        = 0xffc5,
    SOF6        = 0xffc6,
    SOF7        = 0xffc7,
    JPG         = 0xffc8,
    SOF9        = 0xffc9,
    SOF10       = 0xffca,
    SOF11       = 0xffcb,
    DAC         = 0xffcc,
    SOF13       = 0xffcd,
    SOF14       = 0xffce,
    SOF15       = 0xffcf,
    RST0        = 0xffd0,
    RST1        = 0xffd1,
    RST2        = 0xffd2,
    RST3        = 0xffd3,
    RST4        = 0xffd4,
    RST5        = 0xffd5,
    RST6        = 0xffd6,
    RST7        = 0xffd7,
    SOI         = 0xffd8,
    EOI         = 0xffd9,
    SOS         = 0xffda,
    DQT         = 0xffdb,
    DNL         = 0xffdc,
    DRI         = 0xffdd,
    DHP         = 0xffde,
    EXP         = 0xffdf,
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
    COM1        = 0xfffe,
};

struct stMarker {
    std::uint16_t       marker;                     // 0000
    std::uint16_t       length;                     // 0002
};

// 0th IFD
struct st0thIFD {
    std::uint16_t       numOfEntities;
    std::vector<char>   IFDs;
    std::uint32_t       nextIFDOffset;
    std::vector<char>   IFD0thLongerThan4Bytes;
};

// Exif IFD
struct stExifIFD {
    std::uint16_t       numOfExifIFDs;
    std::vector<char>   ExifIFDs;
    std::uint32_t       nextIFDOffset;
    std::vector<char>   ExifIFDLongerThan4Bytes;
};

// 1st IFD
struct st1stIFD {
    std::uint16_t       numOfEntities;
    std::vector<char>   IFDs;
    std::uint32_t       nextIFDOffset;
    std::vector<char>   IFD1stLongerThan4Bytes;
    std::vector<char>   thumbnailData;
};


struct stAPP0 {
    std::uint8_t        JFIFIdentifier[5];          // 0004  "JFIF\0"
    std::uint16_t       version;                    // 0009
    std::uint8_t        pixelConcentrationUnit;     // 000b
    std::uint16_t       pixelConcentrationX;        // 000c
    std::uint16_t       pixelConcentrationY;        // 000e
    //
    std::uint8_t        thumbnailImageWidth;        // 0010
    std::uint8_t        thumbnailImageHeight;       // 0011
    std::vector<char>   thumbnailImage;             // 0012
};

struct stAPP1 {
    std::uint8_t        ExifIdentifier[6];          // 0004  "Exif\0\0"
    std::uint8_t        TIFFByteOrder[2];           // 000a
    std::uint16_t       TIFFIdentifier;             // 000b  0x002a(42)
    std::uint32_t       IFD0thOffset;               // 000d
    //std::vector<char>   IFDs;                       // 0011
    st0thIFD            IFD0th;                     // 0011
    stExifIFD           ExifIFD;
    st1stIFD            IFD1st;
};

struct stAPP2 {
    std::uint8_t        FPXRIdentifierCode[5];      // 0004  "FPXR\0"
    std::uint8_t        version;                    // 0009
    std::vector<char>   streamData;                 // 000a
};

struct stDQT {
    std::uint8_t        tableIdentifier;            // 0004
    std::vector<char>   DQTtable;                   // 0005
};

struct stSOF0 {
    std::uint8_t        samplePrecision;            // 0004
    std::uint16_t       imageRow;                   // 0005
    std::uint16_t       imageColumn;                // 0007
    std::uint8_t        numOfComponents;            // 0009
    std::vector<char>   components;                 // 000a
};

struct stDHT {
    std::uint8_t        DHTClassIdentifier;         // 0004
    std::uint8_t        numOfCodesByCodeLength[16]; // 0005
    std::vector<char>   codesOfEachCodeLength;      // 0015
};

struct stSOS {
    std::uint8_t        numOfComponents;            // 0004
    std::vector<char>   components;                 // 0005
};


