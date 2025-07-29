#include "../../include/services/WhaleTracker.h"
#include "../../include/utils/HexUtils.h"
#include "../../include/utils/Logger.h"
#include <iostream>

WhaleTracker::WhaleTracker(double thresholdEth, DatabaseManager& dbManager)
    : thresholdEth(thresholdEth), dbManager(dbManager) {}

std::vector<Transaction> WhaleTracker::findWhaleTransactions(const Block& block) const {
    std::vector<Transaction> whales;

    for (const auto& tx : block.transactions) {
        try {
            uint256_t wei = hexToUint256(tx.value);
            long double eth = weiToEth(wei);

            if (eth >= thresholdEth) {
                whales.push_back(tx);
            }
        } catch (const std::exception& e) {
            Logger::error(std::string("Error parsing transaction value: ") + e.what());
        }
    }

    if (!whales.empty()) {
        dbManager.insertTransactionsBatch(whales, block.number);
    }

    return whales;
}