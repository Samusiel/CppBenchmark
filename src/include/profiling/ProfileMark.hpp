#pragma once

#include <concepts>
#include <profiling/ProfileMarks.hpp>
#include <string_view>

namespace Profiling {

class ProfileMark {
public:
    consteval static auto create(std::string_view name) -> ProfileMark {
        auto index = getProfileMarkIndex(name);
        assert(index);
        return ProfileMark(name, index.value());
    }

    constexpr auto getName() const -> std::string_view { return _name; }
    constexpr auto getIndex() const -> size_t { return _index; }

private:
    constexpr ProfileMark(std::string_view name, size_t index)
        : _name(name)
        , _index(index) { }

private:
    std::string_view _name;
    size_t _index;
};

} // namespace Profiling