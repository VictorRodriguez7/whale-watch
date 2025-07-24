#include "../../include/services/WhaleTracker.h"
#include "../../include/utils/HexUtils.h"
#include <iostream>

WhaleTracker::WhaleTracker(double thresholdEth) : thresholdEth(thresholdEth) {}

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
            std::cerr << "Error parsing transaction value: " << e.what() << std::endl;
        }
    }

    return whales;
}
