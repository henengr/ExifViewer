#include <string>
#include <fstream>
#include <filesystem>
#include <memory>

#include "JPEGReader.hpp"
#include "IFD.hpp"
#include "IFDReader.hpp"
#include "ReaderFactory.hpp"


namespace fs = std::filesystem;

int main(int argc, char **argv) {
    if (argc != 2) return -1;

    fs::path filePath = argv[1];

    if (!fs::exists(filePath)){
        std::cout << "File not found. :" << filePath.string() << std::endl;
        return -1;
    }

    std::unique_ptr<FileReader> reader = ReaderFactory::create(filePath.string());

    enEndian endian;

    auto pdata = std::make_unique<std::vector<std::uint8_t>>();
    reader->parse(*pdata);

    auto prifd = std::make_unique<IFDReader>(*pdata);

    endian = reader->getEndian();

    auto pifds = std::make_unique<std::vector<stIFD>>();
    prifd->readIFDs(*pifds);

    for (auto &ifd: *pifds){
        //std::cout << std::hex << std::right << std::setw(8) << std::setfill('0') << ifd.offset << " ";
        std::cout << std::hex << std::right << std::setw(4) << std::setfill('0') << ifd.tag << " ";
        std::cout << std::hex << std::right << std::setw(4) << std::setfill('0') << ifd.type << " ";
        std::cout << std::hex << std::right << std::setw(8) << std::setfill('0') << ifd.count << " ";
        //
        std::cout << std::left << std::setfill(' ') << std::setw(30) << ifd.description;
        //
        stIFDValue vals;
        IFDReader::getVariantIFD(ifd, endian, vals);
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::BYTE) ||
            vals.IFD.type == static_cast<uint16_t>(enIFDType::UNDEFINED)){
            std::vector<std::uint8_t> v = std::get<std::vector<std::uint8_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                std::cout << std::hex << std::right <<std::setw(2) << std::setfill('0') << static_cast<int>(*i) << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::ASCII) ||
            vals.IFD.type == static_cast<uint16_t>(enIFDType::UTF8)){
            std::cout << std::get<std::string>(vals.values);
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SHORT)){
            std::vector<std::uint16_t> v = std::get<std::vector<std::uint16_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                std::cout << std::hex << std::right << std::setw(4) << std::setfill('0') << *i << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::LONG)){
            std::vector<std::uint32_t> v = std::get<std::vector<std::uint32_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                std::cout << std::hex << std::right << std::setw(8) << std::setfill('0') << *i << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::RATIONAL)){
            std::vector<stRational> v = std::get<std::vector<stRational>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                std::cout << std::hex << std::right << std::setw(8) << std::setfill('0') << i->long1 << "/";
                std::cout << std::hex << std::right << std::setw(8) << std::setfill('0') << i->long2 << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SLONG)){
            std::vector<std::int32_t> v = std::get<std::vector<std::int32_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                std::cout << std::hex << std::right << std::setw(8) << std::setfill('0') << *i << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SRATIONAL)){
            std::vector<stSRational> v = std::get<std::vector<stSRational>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                std::cout << std::hex << std::right << std::setw(8) << std::setfill('0') << i->slong1 << "/";
                std::cout << std::hex << std::right << std::setw(8) << std::setfill('0') << i->slong2 << " ";
            }
        }
        std::cout << std::endl;

    }

    return 0;
}

