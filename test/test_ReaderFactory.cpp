#include <gtest/gtest.h>
#include <string>
#include <cstdint>
#include <filesystem>

#include "ReaderFactory.hpp"


class ReaderFactoryTest: public ::testing::Test {
protected:
    std::string testPathHEIC;
    std::string testPathJPEG;
    std::string testPathPNG;
    std::string testPathWebP;
    std::string testPathTIFF1;
    std::string testPathTIFF2;

    void SetUp() override{
        std::filesystem::path tempPathHEIC  = std::filesystem::path(testing::TempDir()) / "test_dummy.heic";
        std::filesystem::path tempPathJPEG  = std::filesystem::path(testing::TempDir()) / "test_dummy.jpg";
        std::filesystem::path tempPathPNG   = std::filesystem::path(testing::TempDir()) / "test_dummy.png";
        std::filesystem::path tempPathWebP  = std::filesystem::path(testing::TempDir()) / "test_dummy.webp";
        std::filesystem::path tempPathTIFF1 = std::filesystem::path(testing::TempDir()) / "test_dummy1.tif";
        std::filesystem::path tempPathTIFF2 = std::filesystem::path(testing::TempDir()) / "test_dummy2.tif";
        testPathHEIC  = tempPathHEIC.u8string();
        testPathJPEG  = tempPathJPEG.u8string();
        testPathPNG   = tempPathPNG.u8string();
        testPathWebP  = tempPathWebP.u8string();
        testPathTIFF1 = tempPathTIFF1.u8string();
        testPathTIFF2 = tempPathTIFF2.u8string();

        std::ofstream fileHEIC(tempPathHEIC, std::ios::binary);
        std::ofstream fileJPEG(tempPathJPEG, std::ios::binary);
        std::ofstream filePNG(tempPathPNG, std::ios::binary);
        std::ofstream fileWebP(tempPathWebP, std::ios::binary);
        std::ofstream fileTIFF1(tempPathTIFF1, std::ios::binary);
        std::ofstream fileTIFF2(tempPathTIFF2, std::ios::binary);

        std::vector<std::uint8_t> dummyHEIC = {
            0x00, 0x00, 0x00, 0x34,
            'f', 't', 'y', 'p',
            'h', 'e', 'i', 'c',
            0x00, 0x11, 0x22, 0x33,
            'E', 'x', 'i', 'f',
        };
        std::vector<std::uint8_t> dummyJPEG = {
            0xff, 0xd8, 0xff, 0xe0,
            0x00, 0x14,
            'J', 'F', 'I', 'F',
            0x00, 0x11, 0x22, 0x33,
            'E', 'x', 'i', 'f',
        };
        std::vector<std::uint8_t> dummyPNG = {
            0x89, 'P', 'N', 'G',
            0x0d, 0x0a, 0x1a, 0x0a,
            0x00, 0x00, 0x00, 0x0d,
            'I', 'H', 'D', 'R',
            0x00, 0x11, 0x22, 0x33,
            'e', 'X', 'I', 'f',
        };
        std::vector<std::uint8_t> dummyWebP = {
            'R', 'I', 'F', 'F',
            0x02, 0xba, 0x04, 0x00,
            'W', 'E', 'B', 'P',
            'V', 'P', '8', 'X',
            0x00, 0x11, 0x22, 0x33,
            'E', 'X', 'I', 'F',
        };
        std::vector<std::uint8_t> dummyTIFF1 = {
            'I', 'I', 0x2a, 0x00,
            0x08, 0x00, 0x00, 0x00,
        };
        std::vector<std::uint8_t> dummyTIFF2 = {
            'M', 'M', 0x00, 0x2a,
            0x00, 0x00, 0x00, 0x08,
        };

        fileHEIC.write(reinterpret_cast<const char*>(dummyHEIC.data()), dummyHEIC.size());
        fileJPEG.write(reinterpret_cast<const char*>(dummyJPEG.data()), dummyJPEG.size());
        filePNG.write(reinterpret_cast<const char*>( dummyPNG.data()),  dummyPNG.size());
        fileWebP.write(reinterpret_cast<const char*>(dummyWebP.data()), dummyWebP.size());
        fileTIFF1.write(reinterpret_cast<const char*>(dummyTIFF1.data()), dummyTIFF1.size());
        fileTIFF2.write(reinterpret_cast<const char*>(dummyTIFF2.data()), dummyTIFF2.size());
    }

    void TearDown() override{
        std::filesystem::remove(testPathHEIC);
        std::filesystem::remove(testPathJPEG);
        std::filesystem::remove(testPathPNG);
        std::filesystem::remove(testPathWebP);
        std::filesystem::remove(testPathTIFF1);
        std::filesystem::remove(testPathTIFF2);
    }
};


TEST_F(ReaderFactoryTest, getExtension){
    std::string ext = ReaderFactory::getExtension("/home/test/dev/test.png");
    EXPECT_EQ(ext, "png");
}

TEST_F(ReaderFactoryTest, getExtensionErr){
    std::string ext = ReaderFactory::getExtension("/home/test/dev/test");
    EXPECT_EQ(ext, "");
}


TEST_F(ReaderFactoryTest, checkHeader){
    std::vector<std::uint8_t> exp1 = {0x89, 'P', 'N', 'G'};
    std::vector<std::uint8_t> exp2 = {'e', 'X', 'I', 'f'};

    bool ret = ReaderFactory::checkHeader(testPathPNG, exp1, 16, exp2);
    EXPECT_TRUE(ret);
}

TEST_F(ReaderFactoryTest, checkHeaderErr1){
    std::vector<std::uint8_t> exp1 = {0x80, 'P', 'N', 'G'};     // Invalid value
    std::vector<std::uint8_t> exp2 = {'e', 'X', 'I', 'f'};

    bool ret = ReaderFactory::checkHeader(testPathPNG, exp1, 4, exp2);
    EXPECT_FALSE(ret);
}

TEST_F(ReaderFactoryTest, checkHeaderErr2){
    std::vector<std::uint8_t> exp1 = {0x89, 'P', 'N', 'G'};
    std::vector<std::uint8_t> exp2 = {'E', 'x', 'I', 'f'};      // Invalid value

    bool ret = ReaderFactory::checkHeader(testPathPNG, exp1, 4, exp2);
    EXPECT_FALSE(ret);
}

TEST_F(ReaderFactoryTest, checkHeaderErr3){
    std::vector<std::uint8_t> exp1 = {0x89, 'P', 'N', 'G'};
    std::vector<std::uint8_t> exp2 = {'e', 'X', 'I', 'f'};

    bool ret = ReaderFactory::checkHeader(testPathPNG, exp1, 1000, exp2);
    EXPECT_FALSE(ret);
}

TEST_F(ReaderFactoryTest, checkHeaderErr4){
    std::vector<std::uint8_t> exp1 = {0x89, 'P', 'N', 'G'};
    std::vector<std::uint8_t> exp2 = {'e', 'X', 'I', 'f'};

    bool ret = ReaderFactory::checkHeader("Non-existent.bin", exp1, 4, exp2);
    EXPECT_FALSE(ret);
}


TEST_F(ReaderFactoryTest, createHEIC){
    std::unique_ptr<FileReader> reader = ReaderFactory::create(testPathHEIC);
    EXPECT_TRUE(reader != nullptr);
}

TEST_F(ReaderFactoryTest, createJPEG){
    std::unique_ptr<FileReader> reader = ReaderFactory::create(testPathJPEG);
    EXPECT_TRUE(reader != nullptr);
}

TEST_F(ReaderFactoryTest, createPNG){
    std::unique_ptr<FileReader> reader = ReaderFactory::create(testPathPNG);
    EXPECT_TRUE(reader != nullptr);
}

TEST_F(ReaderFactoryTest, createWebp){
    std::unique_ptr<FileReader> reader = ReaderFactory::create(testPathWebP);
    EXPECT_TRUE(reader != nullptr);
}

TEST_F(ReaderFactoryTest, createTIFF1){
    std::unique_ptr<FileReader> reader = ReaderFactory::create(testPathTIFF1);
    EXPECT_TRUE(reader != nullptr);
}

TEST_F(ReaderFactoryTest, createTIFF2){
    std::unique_ptr<FileReader> reader = ReaderFactory::create(testPathTIFF2);
    EXPECT_TRUE(reader != nullptr);
}
