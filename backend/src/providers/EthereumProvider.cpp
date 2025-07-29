#include "../../include/providers/EthereumProvider.h"
#include "../../include/utils/Logger.h"
#include "../../include/parsers/BlockParser.h"
#include "../../include/utils/HexUtils.h"
#include "../../include/utils/RetryUtils.h"

EthereumProvider::EthereumProvider(const std::string& rpcUrl) : rpcUrl(rpcUrl) {}

std::string EthereumProvider::getChainName() const {
    return "Ethereum Mainnet";
}

std::optional<uint64_t> EthereumProvider::getLatestBlockNumber() {
    std::optional<uint64_t> result;

    retryWithBackoff([&]() {
        Json request = {
            {"jsonrpc", "2.0"},
            {"method", "eth_blockNumber"},
            {"params", Json::array()},
            {"id", 1}
        };

        auto res = httpClient.post(rpcUrl, request.dump());
        if (!res) throw std::runtime_error("HTTP request failed for eth_blockNumber");

        Json json = JsonWrapper::parse(*res);
        if (!json.contains("result")) throw std::runtime_error("Missing 'result' in response");

        result = hexToUint64(json["result"].get<std::string>());
    }, 3, "getLatestBlockNumber");

    return result;
}

std::optional<Block> EthereumProvider::getBlockByNumber(uint64_t blockNumber) {
    std::optional<Block> blockResult;

    retryWithBackoff([&]() {
        std::stringstream ss;
        ss << "0x" << std::hex << blockNumber;

        Json request = {
            {"jsonrpc", "2.0"},
            {"method", "eth_getBlockByNumber"},
            {"params", Json::array({ss.str(), true})},
            {"id", 1}
        };

        auto res = httpClient.post(rpcUrl, request.dump());
        if (!res) throw std::runtime_error("HTTP request failed for eth_getBlockByNumber");

        Json json = JsonWrapper::parse(*res);
        if (!json.contains("result")) throw std::runtime_error("Missing 'result' in response");

        auto parsedBlock = BlockParser::parse(json["result"]);
        if (!parsedBlock) throw std::runtime_error("Failed to parse block data");

        blockResult = parsedBlock;
    }, 3, "getBlockByNumber -> " + std::to_string(blockNumber));

    return blockResult;
}
