#pragma once
#include "../utils/JsonWrapper.h" 
#include "../models/Transaction.h" 
#include <string>
#include <vector>
#include <optional>

class TransactionParser{
public:
    static std::optional<Transaction> parse(const Json& json);
};

