#pragma once

#include <cstdint>
#include <vector>

class IFDReader {
private:
    std::vector<std::uint8_t> ardata;
public:
    explicit IFDReader(){}
    IFDReader(std::vector<std::uint8_t> &data): ardata(data){}
    virtual ~IFDReader(){}
};

