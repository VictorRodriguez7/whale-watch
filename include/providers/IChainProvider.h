#pragma once
#include <string>
#include <optional>
#include "../models/Block.h"

class IChainProvider {
public:
    virtual ~IChainProvider() = default;
    virtual std::optional<uint64_t> getLatestBlockNumber() = 0;
    virtual std::optional<Block> getBlockByNumber(uint64_t blockNumber) = 0;
    virtual std::string getChainName() const = 0;
};

