#pragma once

#include <cstdint>
#include <string_view>
#include <algorithm>

namespace Math {

namespace Compile {

// Compile time string FNV64 hash function
consteval size_t hash(std::string_view str) {
    // A prime number
    size_t hashValue = 151214226848455241;
    constexpr size_t seed = 1099511628211;
    std::for_each(str.begin(), str.end(), [&](auto symbol) {
        hashValue = (hashValue ^ symbol) * seed;
    });
    return hashValue;
}

} // namespace Compile

namespace Runtime {

// Dynamic string FNV64 hash function 
size_t hash(std::string_view str) {
    // A prime number
    size_t hashValue = 151214226848455241;
    constexpr size_t seed = 1099511628211;
    std::for_each(str.begin(), str.end(), [&](auto symbol) {
        hashValue = (hashValue ^ symbol) * seed;
    });
    return hashValue;
}

} // namespace Runtime

} // namespace Math

