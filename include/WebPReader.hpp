#pragma once

#include <cstdint>
#include <fstream>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif
#include "FileReader.hpp"


struct stWebP_VP8XHeader {
    std::string     fourCC;             // "RIFF"
    std::uint32_t   fileSize;
    std::string     webP;               // "WEBP"
    std::string     chunkHeader;        // "VP8X"
    std::uint32_t   payloadSize;
    bool            flagICCProfile;
    bool            flagAlpha;
    bool            flagExif;
    bool            flagXMP;
    bool            flagAnimation;
    std::uint32_t   canvasWidth;
    std::uint32_t   canvasHeight;
};

class WebPReader: public FileReader{
private:
    FRIEND_TEST(WebPReaderTest, readHeader);
    std::int32_t readHeader(std::istream &stream, stWebP_VP8XHeader &header);

    FRIEND_TEST(WebPReaderTest, readChunk);
    std::int32_t readChunk(const std::string fourCC, std::istream &stream, std::vector<std::uint8_t> &data);
protected:
public:
    WebPReader(std::istream &stream):FileReader(stream){} 
    WebPReader(const std::string path):FileReader(path){}
    virtual ~WebPReader(){}

    enEndian getEndian() const override;
    std::int32_t getImageInfo(stImageInfo &info) override;
    std::int32_t parse(std::vector<std::uint8_t> &data) override;
};

