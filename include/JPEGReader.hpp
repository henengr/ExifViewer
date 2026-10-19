#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
#include "FileReader.hpp"


enum class enJPEGSegmentMarker: std::uint16_t {
    ByteStuffing = 0xff00,
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

struct stJPEG_Marker {
    std::uint16_t       marker;                     // 0000
    std::uint16_t       length;                     // 0002
};

struct stJPEG_APP0 {
    char                JFIFIdentifier[5];          // 0004  "JFIF\0"
    std::uint16_t       version;                    // 0009
    std::uint8_t        pixelConcentrationUnit;     // 000b
    std::uint16_t       pixelConcentrationX;        // 000c
    std::uint16_t       pixelConcentrationY;        // 000e
    //
    std::uint8_t        thumbnailImageWidth;        // 0010
    std::uint8_t        thumbnailImageHeight;       // 0011
    std::vector<std::uint8_t> thumbnailImage;       // 0012
};

struct stJPEG_APP1 {
    char                ExifIdentifier[6];          // 0004  "Exif\0\0"
    std::vector<std::uint8_t> data;                 // 000a
};

struct stJPEG_APP2 {
    std::uint8_t        FPXRIdentifierCode[5];      // 0004  "FPXR\0"
    std::uint8_t        version;                    // 0009
    std::vector<std::uint8_t> streamData;           // 000a
};

struct stJPEG_DQT {
    std::uint8_t        tableIdentifier;            // 0004
    std::vector<std::uint8_t> DQTtable;             // 0005
};

struct stJPEG_SOF0 {
    std::uint8_t        samplePrecision;            // 0004
    std::uint16_t       imageRow;                   // 0005
    std::uint16_t       imageColumn;                // 0007
    std::uint8_t        numOfComponents;            // 0009
    std::vector<std::uint8_t> components;           // 000a
};

struct stJPEG_DHT {
    std::uint8_t        DHTClassIdentifier;         // 0004
    std::uint8_t        numOfCodesByCodeLength[16]; // 0005
    std::vector<std::uint8_t> codesOfEachCodeLength;// 0015
};

struct stJPEG_SOS {
    std::uint8_t        numOfComponents;            // 0004
    std::vector<std::uint8_t> components;           // 0005
};


class JPEGReader: public FileReader{
private:
    stJPEG_APP0 app0;
    stJPEG_APP1 app1;

    FRIEND_TEST(JPEGReaderTest, readSegment);
    FRIEND_TEST(JPEGReaderTest, readSegmentErr);
    std::int32_t readSegment(std::uint16_t marker, std::istream &stream, std::vector<std::uint8_t> &data);

    FRIEND_TEST(JPEGReaderTest, parseAPP0);
    FRIEND_TEST(JPEGReaderTest, parseAPP0Err);
    std::int32_t parseAPP0(const std::vector<std::uint8_t>& data, stJPEG_APP0 &value);

    FRIEND_TEST(JPEGReaderTest, parseAPP1);
    FRIEND_TEST(JPEGReaderTest, parseAPP1Err);
    std::int32_t parseAPP1(const std::vector<std::uint8_t>& data, enEndian &endian, stJPEG_APP1 &value);
protected:
public:
    JPEGReader(std::istream &stream):FileReader(stream){} 
    JPEGReader(const std::string path):FileReader(path){}
    virtual ~JPEGReader(){}

    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

