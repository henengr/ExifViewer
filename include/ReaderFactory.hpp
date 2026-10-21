#pragma once

#include <memory>
#ifndef DISABLE_GTEST
#include <gtest/gtest_prod.h>
#else
#define FRIEND_TEST(test_case_name, test_name)
#endif

#include "FileReader.hpp"


class ReaderFactory {
private:
    FRIEND_TEST(ReaderFactoryTest, getExtension);
    FRIEND_TEST(ReaderFactoryTest, getExtensionErr);
    static std::string getExtension(const std::string& path);

    FRIEND_TEST(ReaderFactoryTest, checkHeader);
    FRIEND_TEST(ReaderFactoryTest, checkHeaderErr1);
    FRIEND_TEST(ReaderFactoryTest, checkHeaderErr2);
    FRIEND_TEST(ReaderFactoryTest, checkHeaderErr3);
    FRIEND_TEST(ReaderFactoryTest, checkHeaderErr4);
    static bool checkHeader(const std::string& path, const std::vector<unsigned char>& expectedHeader,
            std::size_t skipByte = 0, const std::vector<unsigned char>& expectedHeader2 = {});
public:
    static std::unique_ptr<FileReader> create(const std::string &path);
};

