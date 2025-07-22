#include "../../include/providers/EthereumProvider.h"
#include "../../include/utils/Logger.h"
#include "../../include/parsers/BlockParser.h"

EthereumProvider::EthereumProvider(const std::string& rpcUrl) : rpcUrl(rpcUrl) {}

std::string EthereumProvider::getChainName() const {
    return "Ethereum Mainnet";
}

std::optional<uint64_t> EthereumProvider::getLatestBlockNumber() {
    Json request = {
        {"jsonrpc", "2.0"},
        {"method", "eth_blockNumber"},
        {"params", Json::array()},
        {"id", 1}
    };

    auto res = httpClient.post(rpcUrl, request.dump());
    if (!res) return std::nullopt;

    Json json = JsonWrapper::parse(*res);
    if (!json.contains("result")) return std::nullopt;

    return std::stoull(json["result"].get<std::string>(), nullptr, 16);
}

std::optional<Block> EthereumProvider::getBlockByNumber(uint64_t blockNumber) {
    std::stringstream ss;
    ss << "0x" << std::hex << blockNumber;

    Json request = {
        {"jsonrpc", "2.0"},
        {"method", "eth_getBlockByNumber"},
        {"params", Json::array({ss.str(), true})},
        {"id", 1}
    };

    auto res = httpClient.post(rpcUrl, request.dump());
    if (!res) return std::nullopt;

    Json json = JsonWrapper::parse(*res);
    if (!json.contains("result")) return std::nullopt;

    return BlockParser::parse(json["result"]);
}
