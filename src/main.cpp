#include "../include/providers/EthereumProvider.h"
#include "../include/utils/Logger.h"
#include <cstdlib>

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
    Logger::info("Transactions:");

    for (const auto& tx : blockOpt->transactions) {
        Logger::info(" - Hash: " + tx.hash);
        Logger::info("   From: " + tx.from);
        Logger::info("   To:   " + (tx.to ? *tx.to : "Contract Creation"));
        Logger::info("   Value: " + tx.value);
    }

    return 0;
}
