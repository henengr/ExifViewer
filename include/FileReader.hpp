#pragma once

#include <vector>
#include <fstream>
#include <cstdint>
#include <memory>
#include <filesystem>


constexpr std::int32_t FILEREADER_FILE_NOT_OPENED                   = -1;
constexpr std::int32_t FILEREADER_INVALID_FILE_SIZE                 = -2;
constexpr std::int32_t FILEREADER_INVALID_FILE_FORMAT               = -3;
constexpr std::int32_t FILEREADER_EXIF_BLOCK_NOT_FOUND              = -8;

class valueConverter {
public:
    static std::uint16_t swapUInt16(const std::uint16_t value);
    static std::uint32_t swapUInt32(const std::uint32_t value);
    static std::uint64_t swapUInt64(const std::uint64_t value);
    //
    static std::uint8_t streamToUInt8(std::istream &ifs);
    static std::uint16_t streamToUInt16(std::istream &ifs);
    static std::uint32_t streamToUInt32(std::istream &ifs);
    static std::uint64_t streamToUInt64(std::istream &ifs);
    static void dispDumb(std::vector<uint8_t> &data);
};

enum class enEndian: std::uint8_t {
    littleEndian = 1,
    bigEndian
};

struct stImageInfo {
    std::uint32_t   width;
    std::uint32_t   height;
    std::uint8_t    bitsPerPixel;
};

class FileReader {
protected:
    std::unique_ptr<std::istream> arstream;
    std::vector<std::uint8_t> ardata;
    enEndian endian;
public:
    FileReader(std::istream &stream): arstream(std::make_unique<std::istream>(stream.rdbuf())), endian(enEndian::littleEndian), ardata(){}
    FileReader(const std::string path): arstream(std::make_unique<std::ifstream>(std::filesystem::u8path(path), std::ios::binary)){}
    virtual ~FileReader() = default;
    virtual enEndian getEndian() const = 0;
    virtual std::int32_t getImageInfo(stImageInfo &info) = 0;
    virtual std::int32_t parse(std::vector<std::uint8_t> &data) = 0;
};

