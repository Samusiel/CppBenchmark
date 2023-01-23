#pragma once

#include <profiling/ProfileMark.hpp>
#include <profiling/Profiler.hpp>
#include <string_view>

namespace Profiling {

using SystemClockTimePoint = std::chrono::time_point<std::chrono::system_clock>;

template <typename Backend>
concept ProfileBackendConcept = requires(Backend&& backend) {
                                    { backend.begin() } -> std::same_as<SystemClockTimePoint>;
                                    { backend.end() } -> std::same_as<SystemClockTimePoint>;
                                    { backend.submit(Profiler::instance(), TimeDuration{}) } -> std::same_as<void>;
                                };

template <ProfileBackendConcept Backend>
class ProfileScopeGuard {
public:
    constexpr ProfileScopeGuard(const Backend& backend)
        : _profiler(Profiler::instance())
        , _backend(backend) {
        _begin = _backend.begin();
    }

    constexpr ~ProfileScopeGuard() {
        _end = _backend.end();
        _backend.submit(_profiler, _end - _begin);
    }

private:
    Profiler& _profiler;
    Backend _backend;
    SystemClockTimePoint _begin;
    SystemClockTimePoint _end;
};

} // namespace Profiling