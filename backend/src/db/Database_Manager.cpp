#include "../../include/db/DatabaseManager.h"
#include "../../include/utils/RetryUtils.h"

DatabaseManager::DatabaseManager(const std::string& connStr) {
    try {
        conn = new pqxx::connection(connStr);
        if (conn->is_open()) {
            Logger::info(std::string("DB Connection established: ") + conn->dbname());
        } else {
            throw std::runtime_error("Failed to connect to DB.");
        }
    } catch (const std::exception& e) {
        Logger::error(std::string("Error initializing DB connection: ") + e.what());
        throw;
    }
}

DatabaseManager::~DatabaseManager() {
    if (conn) {
        conn->close();
        delete conn;
        Logger::info("DB Connection closed.");
    }
}

void DatabaseManager::insertTransaction(const std::string& tx_hash,
                                        const std::string& from_address,
                                        const std::string& to_address,
                                        const std::string& value_eth,
                                        long block_number) {
    try {
        retryWithBackoff([&]() {
            pqxx::work txn(*conn);
            txn.exec(
                "INSERT INTO transactions (tx_hash, from_address, to_address, value_eth, block_number) "
                "VALUES ($1, $2, $3, $4, $5) "
                "ON CONFLICT (tx_hash) DO NOTHING",
                pqxx::params(tx_hash, from_address, to_address, value_eth, block_number)
            ).no_rows();
            txn.commit();
        }, 3, "insertTransaction");
    } catch (const std::exception& e) {
        Logger::error(std::string("Error inserting transaction: ") + e.what());
    }
}

void DatabaseManager::insertTransactionsBatch(const std::vector<Transaction>& txs, long block_number) {
    if (txs.empty()) return;

    try {
        constexpr size_t BATCH_SIZE = 500;  
        size_t totalTxs = txs.size();
        size_t totalBatches = std::ceil((double)totalTxs / BATCH_SIZE);

        for (size_t batchIndex = 0; batchIndex < totalBatches; ++batchIndex) {
            size_t start = batchIndex * BATCH_SIZE;
            size_t end = std::min(start + BATCH_SIZE, totalTxs);

            retryWithBackoff([&]() {
                pqxx::work txn(*conn);

                std::ostringstream query;
                query << "INSERT INTO transactions (tx_hash, from_address, to_address, value_eth, block_number) VALUES ";

                pqxx::params params;
                int paramIndex = 1;

                for (size_t i = start; i < end; ++i) {
                    const auto& tx = txs[i];
                    uint256_t wei = hexToUint256(tx.value);
                    long double eth = weiToEth(wei);
                    std::string ethStr = std::to_string(eth);

                    query << "("
                          << "$" << paramIndex++ << "," // tx_hash
                          << "$" << paramIndex++ << "," // from_address
                          << "$" << paramIndex++ << "," // to_address
                          << "$" << paramIndex++ << "," // value_eth
                          << "$" << paramIndex++        // block_number
                          << ")";
                    if (i < end - 1) query << ",";

                    params.append(tx.hash);
                    params.append(tx.from);
                    params.append(tx.to.value_or(""));
                    params.append(ethStr);
                    params.append(block_number);
                }

                query << " ON CONFLICT (tx_hash) DO NOTHING";

                txn.exec(query.str(), params);
                txn.commit();

                Logger::info("Batch " + std::to_string(batchIndex + 1) + "/" +
                             std::to_string(totalBatches) +
                             " inserted " + std::to_string(end - start) +
                             " transactions for block " + std::to_string(block_number));
            }, 3, "insertTransactionsBatch");
        }
    } catch (const std::exception& e) {
        Logger::error(std::string("Error inserting transaction batch: ") + e.what());
    }
}

void DatabaseManager::setLastProcessedBlock(uint64_t blockNumber) {
    try {
        retryWithBackoff([&]() {
            pqxx::work txn(*conn);
            txn.exec(
                "INSERT INTO meta (key, value) VALUES ('last_block', $1) "
                "ON CONFLICT (key) DO UPDATE SET value = EXCLUDED.value",
                pqxx::params(std::to_string(blockNumber))
            );
            txn.commit();
        }, 3, "setLastProcessedBlock");
    } catch (const std::exception& e) {
        Logger::error(std::string("Error setting last processed block: ") + e.what());
    }
}

std::optional<uint64_t> DatabaseManager::getLastProcessedBlock() {
    std::optional<uint64_t> result;
    try {
        retryWithBackoff([&]() {
            pqxx::work txn(*conn);
            auto res = txn.exec("SELECT value FROM meta WHERE key = 'last_block'");
            txn.commit();
            if (!res.empty()) {
                result = std::stoull(res[0][0].c_str());
            } else {
                Logger::info("No last processed block found in meta table.");
            }
        }, 3, "getLastProcessedBlock");
    } catch (const std::exception& e) {
        Logger::error(std::string("Error retrieving last processsed block: ") + e.what());
    }
    return result;
}

