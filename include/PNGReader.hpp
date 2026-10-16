#pragma once

#include <cstdint>
#include <fstream>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
#include "FileReader.hpp"


enum class enPNG_ColorType: std::uint8_t {
    grayscale               = 0,
    RGBTriple               = 2,
    paletteIndex            = 3,
    grayScaleWithAlpha      = 4,
    RGBTripleWithAlpha      = 6,
};

enum class enPNG_CompressionMethod: std::uint8_t {
    none                    = 0,
    LZ77                    = 8,
};

enum class enPNG_FilterMethod: std::uint8_t {
    none                    = 0,
    sub                     = 1,
    up,
    average,
    paeth,
};

enum class enPNG_InterlaceMethod: std::uint8_t {
    leftToRight_TopToBottom = 0,
    adam7                   = 1,
};

struct stPNG_IHDRData {
    std::uint32_t       width;
    std::uint32_t       height;
    std::uint8_t        bitDepth;
    enPNG_ColorType     colorType;
    enPNG_CompressionMethod compressionMethod;
    enPNG_FilterMethod  filterMethod;
    enPNG_InterlaceMethod interlaceMethod;
};

class PNGReader: public FileReader{
private:
    FRIEND_TEST(PNGReaderTest, readChunk);
    std::int32_t readChunk(const std::string fourCC, std::istream &stream, std::vector<std::uint8_t> &data);
    std::int32_t getIHDRInfo(std::istream &stream, stPNG_IHDRData ihdr);
protected:
public:
    PNGReader(std::istream &stream):FileReader(stream){} 
    PNGReader(const std::string path):FileReader(path){}
    virtual ~PNGReader(){}

    enEndian getEndian() const override;
    std::int32_t getImageInfo(stImageInfo &info) override;
    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

