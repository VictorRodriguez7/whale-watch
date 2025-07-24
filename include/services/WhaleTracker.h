#pragma once
#include <vector>
#include <string>
#include "../models/Block.h"

class WhaleTracker {
public:
    WhaleTracker(double thresholdEth = 10.0);
    std::vector<Transaction> findWhaleTransactions(const Block& block) const;

private:
    double thresholdEth;
};
