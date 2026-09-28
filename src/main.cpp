#include <string>

#include "JFIF.hpp"
#include "JPEGReader.hpp"

using namespace std;

int main(int agrc, char **argv) {
    cout << std::hex << valueConverter::swapUInt64(0x91efcdab87654321) << endl;
    cout << std::hex << valueConverter::swapUInt32(0x87654321) << endl;
    cout << std::hex << valueConverter::swapUInt16(0x4321) << endl;

    return 0;
}

