#pragma once

#include <tuple>
#include <utility>

#include <reflection/MemberCount.hpp>

namespace Reflection {

template <IsAggregate T, typename Visitor>
auto visit(Visitor&& visitor, const T& value) {
    constexpr auto count = memberCount<T>();
    static_assert(count > 0 && count < MaxMemberCount);
    if constexpr (count == 1) {
        auto&& [f] = value;
        return visitor(f);
    } else if constexpr (count == 2) {
        auto&& [f1, f2] = value;
        return visitor(f1, f2);
    } else if constexpr (count == 3) {
        auto&& [f1, f2, f3] = value;
        return visitor(f1, f2, f3);
    } else if constexpr (count == 4) {
        auto&& [f1, f2, f3, f4] = value;
        return visitor(f1, f2, f3, f4);
    } else if constexpr (count == 5) {
        auto&& [f1, f2, f3, f4, f5] = value;
        return visitor(f1, f2, f3, f4, f5);
    } else if constexpr (count == 6) {
        auto&& [f1, f2, f3, f4, f5, f6] = value;
        return visitor(f1, f2, f3, f4, f5, f6);
    } else if constexpr (count == 7) {
        auto&& [f1, f2, f3, f4, f5, f6, f7] = value;
        return visitor(f1, f2, f3, f4, f5, f6, f7);
    } else if constexpr (count == 8) {
        auto&& [f1, f2, f3, f4, f5, f6, f7, f8] = value;
        return visitor(f1, f2, f3, f4, f5, f6, f7, f8);
    } else {
        static_assert((count >= MaxMemberCount) && "MaxMemberCount has been increased, add extra code to support additional values.");
    }
}

template <IsAggregate T>
auto makeTuple(const T& value) {
    return visit([](const auto& ... values) {
        return std::make_tuple(std::forward<decltype(values)>(values)...);
    }, value);
}

} // namespace Reflection