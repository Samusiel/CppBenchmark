#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>

namespace Profiling {

constexpr size_t ProfileMarksCount = 1024;
constexpr std::array<std::string_view, ProfileMarksCount> ProfileMarksNames{"Hello World"};

consteval std::optional<size_t> getProfileMarkIndex(std::string_view name) {
    for (size_t idx = 0; idx < ProfileMarksCount; ++idx) {
        if (name == ProfileMarksNames[idx]) {
            return idx;
        }
    }
    return std::nullopt;
}

} // namespace Profiling