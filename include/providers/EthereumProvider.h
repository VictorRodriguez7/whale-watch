#pragma once
#include "IChainProvider.h"
#include "../utils/HttpClient.h"
#include "../utils/JsonWrapper.h"
#include <sstream>      
#include <optional>     
#include <string>       

class EthereumProvider : public IChainProvider {
public:
    EthereumProvider(const std::string& rpcUrl);

    std::optional<uint64_t> getLatestBlockNumber() override;
    std::optional<Block> getBlockByNumber(uint64_t blockNumber) override;
    std::string getChainName() const override;

private:
    std::string rpcUrl;
    HttpClient httpClient;
};