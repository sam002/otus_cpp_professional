#pragma once

#include <cstring>
#include <new>
#include <stdexcept>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstddef>

template <typename T>
std::string print_ip(T value) {
    auto length = std::max(4, (int)sizeof(T));
    std::array<std::byte, sizeof(T)> bytes{};
    std::copy_n(static_cast<const std::byte *>(static_cast<const void*>(&value)), length, bytes.rbegin());

    std::string addr;
    for (auto byte : bytes) {
        addr += std::to_string(std::to_integer<int>(byte));
        if (byte != bytes.back()) {
            addr += '.';
        }
    }
    return addr;
}
