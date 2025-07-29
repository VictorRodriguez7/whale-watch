#pragma once
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <stdexcept>
#include "../models/Transaction.h" 
#include "../utils/Logger.h"
#include "../utils/HexUtils.h" 


class DatabaseManager {
public:
    DatabaseManager(const std::string& connStr);
    ~DatabaseManager();

    void insertTransaction(const std::string& tx_hash,
                           const std::string& from_address,
                           const std::string& to_address,
                           const std::string& value_eth,
                           long block_number);
    void insertTransactionsBatch(const std::vector<Transaction>& txs, long block_number);
    void setLastProcessedBlock(uint64_t blockNumber);
    std::optional<uint64_t> getLastProcessedBlock();


private:
    pqxx::connection* conn;
};