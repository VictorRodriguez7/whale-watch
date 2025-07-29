#pragma once
#include <string>
#include <cstdint>
#include <boost/multiprecision/cpp_int.hpp>

using uint256_t = boost::multiprecision::uint256_t;

uint256_t hexToUint256(const std::string& hexStr);
uint64_t hexToUint64(const std::string& hexStr);
long double weiToEth(const uint256_t& wei);
