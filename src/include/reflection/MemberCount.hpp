#pragma once

#include <concepts>

namespace Reflection {

struct UniversalType {
    template <typename T>
    operator T();
};

template <typename T>
concept IsAggregate = std::is_aggregate_v<std::remove_cvref_t<T>>;

template <IsAggregate T, typename... Args>
consteval auto memberCount() {
    static_assert(std::is_aggregate_v<std::remove_cvref_t<T>>);

    if constexpr (requires { T{{Args{}}..., {UniversalType{}}}; } == false) {
        return sizeof...(Args);
    } else {
        return memberCount<T, Args..., UniversalType>();
    }
}

constexpr size_t MaxMemberCount = 8;

} // namespace Reflection