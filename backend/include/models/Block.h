#pragma once
#include "Transaction.h"
#include <string>
#include <vector>

struct Block {
    std::string hash;
    uint64_t number;
    std::vector<Transaction> transactions;
};
