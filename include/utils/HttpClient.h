#pragma once
#include <string>
#include <optional>

class HttpClient {
public:
    std::optional<std::string> post(const std::string& url, const std::string& body) const;
};
