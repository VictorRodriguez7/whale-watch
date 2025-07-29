#pragma once
#include <functional>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>

void retryWithBackoff(const std::function<void()>& func,
                      int maxRetries = 3,
                      const std::string& operationName = "");
