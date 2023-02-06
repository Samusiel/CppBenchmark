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

    constexpr auto getName() const noexcept -> std::string_view { return _name; }
    constexpr auto getIndex() const noexcept -> size_t { return _index; }

    constexpr ProfileMark(const ProfileMark& mark) noexcept = default;
    constexpr ProfileMark(ProfileMark&& mark) noexcept = default;

private:
    constexpr ProfileMark(std::string_view name, size_t index) noexcept
        : _name(name)
        , _index(index) { }

private:
    std::string_view _name;
    size_t _index;
};

} // namespace Profiling