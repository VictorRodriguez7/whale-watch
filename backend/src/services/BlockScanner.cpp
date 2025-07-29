#include "../../include/services/BlockScanner.h"
#include "../../include/utils/HexUtils.h"
#include <iostream>
#include <iomanip>
#include <thread>

void BlockScanner::scanBlocks(int startBlock, int numBlocks) {
    for (int i = 0; i < numBlocks; ++i) {
        int currentBlock = startBlock - i;
        try {
            auto blockOpt = provider.getBlockByNumber(currentBlock);

            if (!blockOpt) {
                logger.error("Failed to fetch block " + std::to_string(currentBlock));
                continue;
            }

            const Block& block = *blockOpt;

            auto whaleTxs = tracker.findWhaleTransactions(block);
            if (!whaleTxs.empty()) {
                for (const auto& tx : whaleTxs) {
                    uint256_t wei = hexToUint256(tx.value);
                    long double eth = weiToEth(wei);

                    std::ostringstream oss;
                    oss << std::fixed << std::setprecision(2) << eth;

                    logger.info("   Whale TX Hash: " + tx.hash);
                    logger.info("   From: " + tx.from);
                    logger.info("   To: " + (tx.to ? *tx.to : "Contract Creation"));
                    logger.info("  Value ETH: " + oss.str() + " ETH");
                }
            }

        } catch (const std::exception& e) {
            logger.error("Failed to process block " + std::to_string(currentBlock) + ": " + e.what());
        }
    }
}

void BlockScanner::scanContinuously(uint64_t startBlock, int pollingIntervalSec) {
    auto lastProcessedOpt = db.getLastProcessedBlock();
    uint64_t lastProcessedBlock = lastProcessedOpt.value_or(startBlock);

    while (true) {
        try {
            auto latestBlockOpt = provider.getLatestBlockNumber();
            if (!latestBlockOpt) {
                logger.error("Failed to fetch latest block number.");
                std::this_thread::sleep_for(std::chrono::seconds(pollingIntervalSec));
                continue;
            }

            uint64_t latestBlock = *latestBlockOpt;
            if (latestBlock <= lastProcessedBlock) {
                logger.info("No new blocks to process. Waiting for " + std::to_string(pollingIntervalSec) + " seconds...");
                std::this_thread::sleep_for(std::chrono::seconds(pollingIntervalSec));
                continue;
            }

            scanBlocks(lastProcessedBlock + 1, latestBlock - lastProcessedBlock);
            lastProcessedBlock = latestBlock;
            db.setLastProcessedBlock(lastProcessedBlock);

        } catch (const std::exception& e) {
            logger.error("Error during continuous scanning: " + std::string(e.what()));
        }

        std::this_thread::sleep_for(std::chrono::seconds(pollingIntervalSec));
    }
}



