#pragma once

#include <chrono>
#include <concepts>
#include <profiling/ProfileMark.hpp>
#include <profiling/Profiler.hpp>
#include <utility>

namespace Profiling {

using ProfilingClock = std::chrono::system_clock;
using ProfilingTimePoint = std::chrono::time_point<ProfilingClock>;

template <typename Begin, typename End, typename Update, typename Submit>
struct ProfileBackend {
    constexpr ProfileBackend(Begin&& begin_, End&& end_, Update&& update_, Submit&& submit_)
        : begin(std::move(begin_))
        , end(std::move(end_))
        , update(std::move(update_))
        , submit(std::move(submit_)) { }
    Begin begin;
    End end;
    Update update;
    Submit submit;
};

class ProfileBackendFactory {
public:
    static consteval auto createCPUBackend(ProfileMark mark) {
        return ProfileBackend(
            [] {
                return ProfilingClock::now();
            },
            [] {
                return ProfilingClock::now();
            },
            [] {},
            [=](Profiler& profiler, const TimeDuration& duration) {
                profiler.emit(Timer{mark, duration});
            });
    }

    static consteval auto createMockBackend() {
        return ProfileBackend(
            [] {
                return ProfilingClock::now();
            },
            [] {
                return ProfilingClock::now();
            },
            [] {}, [](Profiler&, const TimeDuration&) {});
    }
};

using CPUBackend = decltype(ProfileBackendFactory::createCPUBackend(std::declval<ProfileMark>()));
using MockBackend = decltype(ProfileBackendFactory::createMockBackend());

} // namespace Profiling