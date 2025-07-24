#pragma once
#include <string>
#include <cstdint>
#include <boost/multiprecision/cpp_int.hpp>

using uint256_t = boost::multiprecision::uint256_t;

// Convert hex string to uint256_t
uint256_t hexToUint256(const std::string& hexStr);

// Convert Wei (uint256) to ETH as long double
long double weiToEth(const uint256_t& wei);
