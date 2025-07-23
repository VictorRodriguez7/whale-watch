#include "../../include/utils/HexUtils.h"
#include <sstream>
#include <stdexcept>

uint64_t hexToUint64(const std::string& hexStr) {
    std::string cleanHex = hexStr;
    if (cleanHex.rfind("0x", 0) == 0) {
        cleanHex = cleanHex.substr(2); // Remove 0x prefix
    }

    uint64_t value = 0;
    std::stringstream ss;
    ss << std::hex << cleanHex;
    ss >> value;

    if (ss.fail()) {
        throw std::runtime_error("Invalid hex string: " + hexStr);
    }
    return value;
}

double weiToEth(uint64_t wei) {
    return static_cast<double>(wei) / 1e18;
}