#include "../../include/parsers/TransactionParser.h"

std::optional<Transaction> TransactionParser::parse(const Json& txJson) {
    if (!txJson.contains("hash") || !txJson.contains("from") || !txJson.contains("value")) {
        return std::nullopt;
    }

    Transaction tx;
    tx.hash = txJson["hash"].get<std::string>();
    tx.from = txJson["from"].get<std::string>();
    tx.value = txJson["value"].get<std::string>();

    if (txJson.contains("to") && !txJson["to"].is_null()) {
        tx.to = txJson["to"].get<std::string>();
    }

    return tx;
}
