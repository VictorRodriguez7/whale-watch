#include "../include/providers/EthereumProvider.h"
#include "../include/utils/Logger.h"
#include "../include/utils/HexUtils.h"
#include "../include/services/WhaleTracker.h"
#include "../include/services/BlockScanner.h"
#include "../include/db/DatabaseManager.h"  
#include "../include/config/ConfigManager.h"
#include <cstdlib>
#include <iomanip>

int main(int argc, char* argv[]) {
    Logger logger;

    try {
        const char* rpcUrl = std::getenv("RPC_URL");
        if (!rpcUrl) throw std::runtime_error("RPC_URL environment variable is not set.");
        EthereumProvider provider(rpcUrl);

        const char* dbUrl = std::getenv("DATABASE_URL");
        if (!dbUrl) throw std::runtime_error("DATABASE_URL not set");
        DatabaseManager db(dbUrl);

        double threshold = 1.0;
        if (const char* thresholdEnv = std::getenv("THRESHOLD_ETH")) threshold = std::stod(thresholdEnv);
        WhaleTracker tracker(threshold, db);
        BlockScanner scanner(provider, tracker, db, logger);

        std::optional<uint64_t> latestBlockOpt = provider.getLatestBlockNumber();
        if (!latestBlockOpt) {
            logger.error("Failed to fetch latest block number.");
            return EXIT_FAILURE;
        }

        uint64_t latestBlock = *latestBlockOpt;
        logger.info("Latest Block: " + std::to_string(latestBlock));
        logger.info("Starting continuous scan mode...");
        int pollingIntervalSec = 10;
        scanner.scanContinuously(latestBlock, pollingIntervalSec);
       

    } catch (const std::exception& e) {
        logger.error("Error: " + std::string(e.what()));
        return EXIT_FAILURE;
    }
}
