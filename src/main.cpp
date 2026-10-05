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
        cout << hex << setw(4) << setfill('0') << ifd.tag << " ";
        cout << hex << setw(4) << setfill('0') << ifd.type << " ";
        cout << hex << setw(8) << setfill('0') << ifd.count << " ";
        //
        stIFDValue vals;
        IFDReader::getVariantIFD(ifd, enEndian::bigEndian, vals);
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::BYTE)){
            //cout << hex << setfill('0') << get<vector
        }
        if (vals.IFD.type == static_cast<uint16_t>(enIFDType::ASCII)){
            cout << get<string>(vals.values);
            //string str(get<string>(vals.values), ifd.count);
            //cout << str;
        }
        cout << " - " << ifd.description;
        cout << endl;

    }

    file.close();
    return 0;
}

