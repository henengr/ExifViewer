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
    }

    auto pjpg = std::make_unique<JPEGReader>(file);

    uint16_t marker;
    uint16_t length;
    vector<uint8_t> data;
    // SOI
    pjpg->getMarker(marker);    // SOI
    cout << "marker:" << std::hex << marker << endl;

    pjpg->getMarker(marker);    // APP0
    cout << "marker:" << std::hex << marker << endl;
    pjpg->getLength(length);
    cout << "length:" << std::hex << length << endl;
    pjpg->readSegment(length, data);

    pjpg->getMarker(marker);    // APP1
    cout << "marker:" << std::hex << marker << endl;
    pjpg->getLength(length);
    cout << "length:" << std::hex << length << endl;
    pjpg->readSegment(length, data);

    auto dtb = data.begin();
    data.erase(dtb, dtb + 6);   // このやり方はよくない。あとで直す。 TBD

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

