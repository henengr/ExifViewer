#include <string>
#include <algorithm>
#include <fstream>
#include <memory>
#include <vector>
#include <filesystem>

#include "ReaderFactory.hpp"
#include "JPEGReader.hpp"
#include "HEICReader.hpp"


std::string ReaderFactory::getExtension(const std::string& path) {
    size_t dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos) return "";
    std::string ext = path.substr(dotPos + 1);
    //std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

bool ReaderFactory::checkHeader(const std::string& path, const std::vector<unsigned char>& expectedHeader){
    std::filesystem::path filePath = std::filesystem::u8path(path);

    std::ifstream file(filePath, std::ios::binary);
    if (!file) return false;

    std::vector<unsigned char> actualHeader(expectedHeader.size());
    file.read(reinterpret_cast<char*>(actualHeader.data()), expectedHeader.size());
    
    if (file.gcount() < static_cast<std::streamsize>(expectedHeader.size())) return false;
    return actualHeader == expectedHeader;
}


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

    return nullptr;
}

