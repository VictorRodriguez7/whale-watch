#include "../../include/parsers/BlockParser.h"
#include "../../include/utils/Logger.h"
#include <sstream>

std::optional<Block> BlockParser::parse(const Json& json) {
    try {
        Block block;
        block.hash = json.at("hash").get<std::string>();
        block.number = std::stoull(json.at("number").get<std::string>(), nullptr, 16);

        for (const auto& tx : json.at("transactions")) {
            Transaction t;
            t.hash = tx.value("hash", "");
            t.from = tx.value("from", "");
            if (tx.contains("to") && !tx["to"].is_null())
                t.to = tx["to"].get<std::string>();
            t.value = tx.value("value", "0x0");

            block.transactions.push_back(t);
        }

        return block;
    } catch (const std::exception& e) {
        Logger::error("BlockParser failed: " + std::string(e.what()));
        return std::nullopt;
    }
}
