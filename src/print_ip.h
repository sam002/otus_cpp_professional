#pragma once

#include <cstring>
#include <new>
#include <stdexcept>
#include <memory>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstddef>

template<typename T>
using vector_array = std::vector<std::string>;

template <typename T>
typename std::enable_if<std::is_integral_v<T>, vector_array<T>>::type parse2b(T value) {
    std::array<std::byte, sizeof(T)> bytes{};
    std::copy_n(static_cast<const std::byte *>(static_cast<const void*>(&value)), sizeof(T), bytes.rbegin());

    vector_array<T> chars{};
    for (auto byte : bytes) {
        chars.push_back(std::to_string(std::to_integer<int>(byte)));
    }
    return chars;
}

template <typename T>
typename std::enable_if<std::is_integral_v<typename T::value_type>, vector_array<typename T::value_type>>::type parse2b(const T & value) {
    vector_array<typename T::value_type> strings{};

    for(auto item : value) {
        strings.push_back(std::to_string(item));
    }
    return strings;
}

template <typename T>
std::string print_ip(T value) {
    if constexpr (std::is_same_v<T, std::string>) {
        return value;
    } else {
        auto bytes = parse2b<T>(value);
        std::string addr;
        for (const auto &byte : bytes) {
            addr += byte;
            if (byte != bytes.back()) {
                addr += '.';
            }
        }

        return addr;
    }
}

