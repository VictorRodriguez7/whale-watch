#include "../../include/parsers/BlockParser.h"
#include "../../include/parsers/TransactionParser.h"
#include "../../include/utils/HexUtils.h"

std::optional<Block> BlockParser::parse(const Json& json) {
    if (!json.contains("hash") || !json.contains("transactions")) {
        return std::nullopt;
    }

    Block block;
    block.hash = json["hash"].get<std::string>();
    block.number = hexToUint64(json["number"].get<std::string>());

    if (json["transactions"].is_array()) {
        for (const auto& txJson : json["transactions"]) {
            auto tx = TransactionParser::parse(txJson);
            if (tx) {
                block.transactions.push_back(*tx);
            }
        }
    }

    return block;
}
