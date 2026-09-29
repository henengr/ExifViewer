#include <string>
#include <iostream>
#include <memory>

#include "JFIF.hpp"
#include "JPEGReader.hpp"

using namespace std;

int main(int argc, char **argv) {
    cout << "file:" << argv[1] << endl;

    auto pfs = std::make_unique<ifstream>();
    pfs->open(argv[1], std::ios::binary);

    auto pjpg = std::make_unique<JPEGReader>(*pfs);

    uint16_t marker;
    uint16_t length;
    vector<char> data;
    pjpg->getMarker(marker);    // SOI
    cout << "marker:" << std::hex << length << endl;
    pjpg->getMarker(marker);    // APP0
    cout << "marker:" << std::hex << length << endl;
    pjpg->getLength(length);
    cout << "length:" << std::hex << length << endl;
    pjpg->readSegment(length, data);

    //valueConverter::dispDumb(data);

    //cout << "SOI marker:" << std::hex << soi.marker << endl;

    return 0;
}

