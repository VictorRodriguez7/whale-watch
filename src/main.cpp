#include "../include/providers/EthereumProvider.h"
#include "../include/utils/Logger.h"
#include "../include/utils/HexUtils.h"
#include "../include/services/WhaleTracker.h"
#include "../include/services/BlockScanner.h"
#include <cstdlib>
#include <iomanip>

int main() {
    const char* envUrl = std::getenv("INFURA_URL");
    if (!envUrl) {
        Logger logger;
        logger.error("INFURA_URL not set in environment.");
        return 1;
    }

    Logger logger;
    EthereumProvider provider(envUrl);
    WhaleTracker tracker(0.5); // Threshold = 0.5 ETH
    BlockScanner scanner(provider, tracker, logger);

    logger.info("Fetching latest block number...");
    auto latestBlockNum = provider.getLatestBlockNumber();
    if (!latestBlockNum) {
        logger.error("Failed to fetch latest block number.");
        return 1;
    }

    logger.info("Latest block: " + std::to_string(*latestBlockNum));

    int numBlocks = 5; // Scan last 5 blocks
    logger.info("Scanning last " + std::to_string(numBlocks) + " blocks for whale transactions...");

    scanner.scanBlocks(*latestBlockNum, numBlocks);

    return 0;
    
}
