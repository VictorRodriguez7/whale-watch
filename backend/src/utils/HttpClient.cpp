#include "../../include/utils/HttpClient.h"
#include <cpr/cpr.h>

std::optional<std::string> HttpClient::post(const std::string& url, const std::string& body) const {
    cpr::Response r = cpr::Post(
        cpr::Url{url},
        cpr::Body{body},
        cpr::Header{{"Content-Type", "application/json"}}
    );

    if (r.status_code != 200) return std::nullopt;
    return r.text;
}
