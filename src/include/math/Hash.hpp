#pragma once

#include <cstdint>
#include <string_view>
#include <algorithm>

namespace Math {

constexpr size_t hash(std::string_view str) {
    // A prime number
    size_t hashValue = 151214226848455241;
    constexpr size_t seed = 1099511628211;
    std::for_each(str.begin(), str.end(), [&](auto symbol) {
        hashValue = (hashValue ^ symbol) * seed;
    });
    return hashValue;
}

consteval size_t compileTimeHash(std::string_view str) {
    return hash(str);
}

} // namespace Math

