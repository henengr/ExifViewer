#pragma once

#include <memory>

#include "FileReader.hpp"


class ReaderFactory {
private:
    static std::string getExtension(const std::string& path);
    static bool checkHeader(const std::string& path, const std::vector<unsigned char>& expectedHeader);
public:
    static std::unique_ptr<FileReader> create(const std::string &path);
};

