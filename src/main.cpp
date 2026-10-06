#include <string>
#include <fstream>
#include <filesystem>
#include <memory>

#include "JPEGReader.hpp"
#include "JFIF.hpp"
#include "IFD.hpp"
#include "IFDReader.hpp"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char **argv) {
    if (argc != 2) return -1;

    fs::path filePath = argv[1];

    if (!fs::exists(filePath)){
        cout << "File not found. :" << filePath.string() << endl;
        return -1;
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()){
        cout << "File could not be opened. :" << filePath.string() << endl;
        return -1;
    }

    auto pjpg = std::make_unique<JPEGReader>(file);

    std::int32_t ret;
    enEndian endian;
    stAPP1 app1;

    ret = pjpg->readSegment(static_cast<std::uint16_t>(enSegmentMarker::APP1));
    if (ret < 0){
        cout << "Invalid file. : " << filePath.string() << endl;
        return -1;
    }

    ret = pjpg->parseAPP1(endian, app1);
    if (ret < 0){
        cout << "File parsing error. " << filePath.string() << endl;
        return -1;
    }

    vector<uint8_t> &data = app1.data;
    auto rifd = std::make_unique<IFDReader>(data);

    std::vector<stIFD> ifds;
    //auto ifds = std::make_unique<std::vector<stIFD>>(stIFD);
    rifd->readIFDs(ifds);

    for (stIFD ifd: ifds){
        //cout << hex << right << setw(8) << setfill('0') << ifd.offset << " ";
        cout << hex << right << setw(4) << setfill('0') << ifd.tag << " ";
        cout << hex << right << setw(4) << setfill('0') << ifd.type << " ";
        cout << hex << right << setw(8) << setfill('0') << ifd.count << " ";
        //
        cout << left << setfill(' ') << setw(30) << ifd.description;
        //
        stIFDValue vals;
        IFDReader::getVariantIFD(ifd, endian, vals);
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::BYTE) ||
            vals.IFD.type == static_cast<uint16_t>(enIFDType::UNDEFINED)){
            vector<uint8_t> v = get<vector<uint8_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                cout << hex << right <<setw(2) << setfill('0') << static_cast<int>(*i) << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::ASCII) ||
            vals.IFD.type == static_cast<uint16_t>(enIFDType::UTF8)){
            cout << get<string>(vals.values);
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SHORT)){
            vector<uint16_t> v = get<vector<uint16_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                cout << hex << right <<setw(4) << setfill('0') << *i << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::LONG)){
            vector<uint32_t> v = get<vector<uint32_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                cout << hex << right <<setw(8) << setfill('0') << *i << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::RATIONAL)){
            vector<stRational> v = get<vector<stRational>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                cout << hex << right <<setw(8) << setfill('0') << i->long1 << "/";
                cout << hex << right <<setw(8) << setfill('0') << i->long2 << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SLONG)){
            vector<int32_t> v = get<vector<int32_t>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                cout << hex << right <<setw(8) << setfill('0') << *i << " ";
            }
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::SRATIONAL)){
            vector<stSRational> v = get<vector<stSRational>>(vals.values);
            for (auto i = v.begin(); i != v.end(); i++){
                cout << hex << right <<setw(8) << setfill('0') << i->slong1 << "/";
                cout << hex << right <<setw(8) << setfill('0') << i->slong2 << " ";
            }
        }
        cout << endl;

    }

    file.close();
    return 0;
}

