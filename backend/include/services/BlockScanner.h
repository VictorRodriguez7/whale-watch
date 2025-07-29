#pragma once
#include "../providers/EthereumProvider.h"
#include "../parsers/BlockParser.h"
#include "../utils/Logger.h"
#include "WhaleTracker.h"

class BlockScanner {
public:
    BlockScanner(EthereumProvider& provider, WhaleTracker& tracker, DatabaseManager& db, Logger& logger)
        : provider(provider), tracker(tracker), db(db), logger(logger) {}
    void scanBlocks(int startBlock, int numBlocks);
    void scanContinuously(uint64_t startBlock, int pollingIntervalSec);

private:
    EthereumProvider& provider;
    WhaleTracker& tracker;
    Logger& logger;
    DatabaseManager& db;
};
