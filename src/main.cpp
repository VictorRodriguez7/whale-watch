#include "../include/providers/EthereumProvider.h"
#include "../include/utils/Logger.h"
#include "../include/utils/HexUtils.h"
#include "../include/services/WhaleTracker.h"
#include <cstdlib>
#include <iomanip>

int main() {
    const char* envUrl = std::getenv("INFURA_URL");
    if (!envUrl) {
        Logger::error("INFURA_URL not set in environment.");
        return 1;
    }

    EthereumProvider provider(envUrl);

    Logger::info("Fetching latest block number...");
    auto latestBlockNum = provider.getLatestBlockNumber();
    if (!latestBlockNum) {
        Logger::error("Failed to fetch latest block number.");
        return 1;
    }

    Logger::info("Latest block: " + std::to_string(*latestBlockNum));

    Logger::info("Fetching block data...");
    auto blockOpt = provider.getBlockByNumber(*latestBlockNum);
    if (!blockOpt) {
        Logger::error("Failed to fetch block.");
        return 1;
    }

    Logger::info("Block hash: " + blockOpt->hash);
    
    WhaleTracker tracker(0.5); // 0.5 ETH threshold
    auto whaleTxs = tracker.findWhaleTransactions(*blockOpt);
    if (whaleTxs.empty()) {
        Logger::info("No whale transactions found in this block.");
    } else {
        Logger::info("Whale transactions found:");
        for (const auto& whaleTx : whaleTxs) {
            uint256_t wei = hexToUint256(whaleTx.value);
            long double eth = weiToEth(wei);

            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << eth; 

            Logger::info(" - Whale Tx Hash: " + whaleTx.hash);
            Logger::info("   From: " + whaleTx.from);
            Logger::info("   To:   " + (whaleTx.to ? *whaleTx.to : "Contract Creation"));
            Logger::info("   Value: " + whaleTx.value);
            Logger::info("   Value ETH: " + oss.str() + " ETH");
        }
    }

    return 0;
}
