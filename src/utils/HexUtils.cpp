#include "../../include/utils/HexUtils.h"
#include <sstream>
#include <stdexcept>

uint256_t hexToUint256(const std::string& hexStr) {
    std::string cleanHex = (hexStr.rfind("0x", 0) == 0) ? hexStr.substr(2) : hexStr;
    if (cleanHex.empty()) {
        throw std::runtime_error("Empty hex string");
    }

    uint256_t value;
    std::stringstream ss;
    ss << std::hex << cleanHex;
    ss >> value;

    if (ss.fail()) {
        throw std::runtime_error("Invalid hex string: " + hexStr);
    }

    return value;
}

long double weiToEth(const uint256_t& wei) {
    static const uint256_t weiPerEth("1000000000000000000"); // 1 ETH = 1e18 Wei
    return static_cast<long double>(wei.convert_to<long double>() / 1e18);
}