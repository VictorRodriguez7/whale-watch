#include "../../include/services/BlockScanner.h"
#include "../../include/utils/HexUtils.h"
#include <iostream>
#include <iomanip>

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
            logger.info("Processing Block: " + std::to_string(currentBlock));

            auto whaleTxs = tracker.findWhaleTransactions(block);
            if (!whaleTxs.empty()) {
                logger.info("Found " + std::to_string(whaleTxs.size()) +
                            " whale transactions in block " + std::to_string(currentBlock));

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

