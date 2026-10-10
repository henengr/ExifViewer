#pragma once

#include <vector>
#include <fstream>
#include <cstdint>
#include <filesystem>


constexpr std::int32_t FILEREADER_INVALID_FILE_SIZE                 = -1;
constexpr std::int32_t FILEREADER_INVALID_FILE_FORMAT               = -2;

enum class enEndian: std::uint8_t {
    littleEndian = 1,
    bigEndian
};

class FileReader {
protected:
    std::ifstream arstream;
    std::vector<std::uint8_t> ardata;
    enEndian endian;
public:
    FileReader(std::ifstream &stream): arstream(std::move(stream)), endian(enEndian::littleEndian), ardata(){}
    FileReader(const std::string path): arstream(std::filesystem::u8path(path), std::ios::binary){}
    virtual ~FileReader() = default;
    virtual enEndian getEndian() const = 0;
    virtual std::int32_t parse(std::vector<std::uint8_t> &data) = 0;
};

