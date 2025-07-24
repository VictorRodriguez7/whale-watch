#pragma once
#include "../providers/EthereumProvider.h"
#include "../parsers/BlockParser.h"
#include "../utils/Logger.h"
#include "WhaleTracker.h"

class BlockScanner {
public:
    BlockScanner(EthereumProvider& provider,
                 WhaleTracker& tracker,
                 Logger& logger)
        : provider(provider), tracker(tracker), logger(logger) {}

    void scanBlocks(int startBlock, int numBlocks);

private:
    EthereumProvider& provider;
    WhaleTracker& tracker;
    Logger& logger;
};
