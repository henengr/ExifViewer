#include <string>
#include <algorithm>
#include <fstream>
#include <memory>
#include <vector>
#include <filesystem>

#include "ReaderFactory.hpp"
#include "JPEGReader.hpp"
#include "HEICReader.hpp"
#include "PNGReader.hpp"
#include "WebPReader.hpp"


/// @brief Get file extension
/// @param[in]      path    Path to the file
/// @return Return the file extension
std::string ReaderFactory::getExtension(const std::string& path) {
    size_t dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos) return "";
    std::string ext = path.substr(dotPos + 1);
    //std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

/// @brief Check the basic header structure to determine the file type
/// @param[in]      path            Path to the image file
/// @param[in]      expectedHeader  Expected header bytes #1
/// @param[in]      skipByte        Number of bytes to skip between "expectedHeader" and "expectedHeader2"
/// @param[in]      expectedHeader2 Expected header bytes #1
/// @return Whether the file type matches
bool ReaderFactory::checkHeader(const std::string& path, const std::vector<unsigned char>& expectedHeader,
        std::size_t skipByte, const std::vector<unsigned char>& expectedHeader2){
    std::filesystem::path filePath = std::filesystem::u8path(path);

    std::ifstream file(filePath, std::ios::binary);
    if (!file) return false;

    std::vector<unsigned char> actualHeader(expectedHeader.size());
    std::vector<unsigned char> actualHeader2(expectedHeader2.size());
    file.read(reinterpret_cast<char*>(actualHeader.data()), expectedHeader.size());

    if (file.gcount() < static_cast<std::streamsize>(expectedHeader.size())) return false;

    if (skipByte > 0) file.seekg(skipByte, std::ios::cur);
    if (expectedHeader2.size() > 0)
            file.read(reinterpret_cast<char*>(actualHeader2.data()), expectedHeader2.size());
    
    return (actualHeader == expectedHeader && actualHeader2 == expectedHeader2);
}


/// @brief *Reader class object creation method
/// @param[in]      path    File path for an image file
/// @return                 *Reader class object instance
std::unique_ptr<FileReader> ReaderFactory::create(const std::string &path){
    std::string ext = getExtension(path);

    // JPEG
    if (ext == "jpg" || ext == "jpeg" || ext == "JPG" || ext == "JPEG"){
        if (checkHeader(path, {0xff, 0xd8})){
            return std::make_unique<JPEGReader>(path);
        }
    }

    // HEIC
    if (ext == "heic" || ext == "HEIC"){
        if (checkHeader(path, {0x00, 0x00, 0x00, '4', 'f', 't', 'y', 'p', 'h', 'e', 'i', 'c'})){
            return std::make_unique<HEICReader>(path);
        }
    }

    // PNG
    if (ext == "png" || ext == "PNG"){
        if (checkHeader(path, {0x89, 'P', 'N', 'G', 0x0d, 0x0a, 0x1a, 0x0a})){
            return std::make_unique<PNGReader>(path);
        }
    }

    // WebP
    if (ext == "webp" || ext == "WEBP" || ext == "WebP"){
        if (checkHeader(path, {'R', 'I', 'F', 'F'}, 4, {'W', 'E', 'B', 'P', 'V', 'P', '8', 'X'})){
            return std::make_unique<WebPReader>(path);
        }
    }

    return nullptr;
}

