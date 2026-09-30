#include <string>
#include <iostream>
#include <memory>

#include "JFIF.hpp"
#include "IFD.hpp"
#include "JPEGReader.hpp"

using namespace std;

int main(int argc, char **argv) {
    cout << "file:" << argv[1] << endl;

    auto pfs = std::make_unique<ifstream>();
    pfs->open(argv[1], std::ios::binary);

    auto pjpg = std::make_unique<JPEGReader>(*pfs);

    uint16_t marker;
    uint16_t length;
    vector<uint8_t> data;
    // SOI
    pjpg->getMarker(marker);    // SOI
    cout << "marker:" << std::hex << marker << endl;
    cout << endl;
    // APP0
    pjpg->getMarker(marker);    // APP0
    cout << "marker:" << std::hex << marker << endl;
    pjpg->getLength(length);
    cout << "length:" << std::hex << length << endl;
    pjpg->readSegment(length, data);
    //valueConverter::dispDumb(data);
    stAPP0 app0;
    pjpg->getAPP0(length, app0, data);
    cout << "JFIF marker:" << app0.JFIFIdentifier << endl;
    cout << "version:" << app0.version << endl;
    cout << "pixelConcentrationUnit:" << app0.pixelConcentrationUnit << endl;
    cout << "pixelConcentrationX:" << app0.pixelConcentrationX << endl;
    cout << "pixelConcentrationY:" << app0.pixelConcentrationY << endl;
    cout << endl;
    //
    pjpg->getMarker(marker);    // APP1
    cout << "marker:" << std::hex << marker << endl;
    pjpg->getLength(length);
    cout << "length:" << std::hex << length << endl;
    pjpg->readSegment(length, data);
    //valueConverter::dispDumb(data);
    stAPP1 app1;
    pjpg->getAPP1(length, app1, data);
    cout << "Exif marker:" << app1.ExifIdentifier << endl;
    cout << "TIFF ByteOrder:" << app1.TIFFByteOrder << endl;
    cout << "TIFF Identifier:" << app1.TIFFIdentifier << endl;
    cout << "0th IFD Offset:" << app1.IFD0thOffset << endl;
    cout << endl;

    pfs->close();
    return 0;
}

