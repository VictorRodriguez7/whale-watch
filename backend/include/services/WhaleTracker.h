#pragma once
#include <vector>
#include <string>
#include "../models/Block.h"
#include "../db/DatabaseManager.h"

class WhaleTracker {
public:
    WhaleTracker(double thresholdEth, DatabaseManager& dbManager);
    std::vector<Transaction> findWhaleTransactions(const Block& block) const;

private:
    double thresholdEth;
    DatabaseManager& dbManager;
};
