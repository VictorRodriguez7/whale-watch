#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

int main() {
    
    const char* url = std::getenv("INFURA_URL");
    if (!url) {
        std::cerr << "INFURA_URL not set in environment." << std::endl;
        return 1;
    }
    std::string infura_url = url;

    nlohmann::json payload = {
        {"jsonrpc", "2.0"},
        {"method", "eth_getBlockByNumber"},
        {"params", {"0x103f1e0", true}},  // 0x103f1e0 = 17000000
        {"id", 1}
    };

    cpr::Response response = cpr::Post(
        cpr::Url{infura_url},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{payload.dump()}
    );

    if (response.status_code == 200) {
        nlohmann::json result = nlohmann::json::parse(response.text);
        nlohmann::json block = result["result"];
        if (!block.contains("transactions")) {
            std::cerr << "No transactions found in block." << std::endl;
            return 1;
        }

        for (const auto& tx : block["transactions"]) {
            std::string from = tx.value("from", "N/A");
            std::string to = tx.value("to", "N/A");
            std::string value_hex = tx.value("value", "0x0");

            double eth = 0.0;

            try {
                unsigned long long wei = std::stoull(value_hex.substr(2), nullptr, 16);
                eth = static_cast<double>(wei) / 1e18;
            } catch (const std::exception& e) {
                std::cerr << "Failed to parse value for tx from " << from
                          << " → " << to << " | Error: " << e.what() << std::endl;
            }

            std::cout << "From: " << from
                      << " → To: " << to
                      << " | Value: " << eth << " ETH" << std::endl;

        }

    } else {
        std::cerr << "Failed to fetch block. Status code: " << response.status_code << std::endl;
        std::cerr << response.text << std::endl;
    }

    return 0;
}
