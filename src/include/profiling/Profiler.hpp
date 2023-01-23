#pragma once

#include <profiling/ProfileMark.hpp>

namespace Profiling {

using TimeDuration = std::chrono::nanoseconds;

struct Timer {
    ProfileMark mark;
    TimeDuration duration;
};

class Profiler {
public:
    // ToDo: remove this method and put profiler under the App class
    static Profiler& instance() {
        static Profiler profiler;
        return profiler;
    }

    void emit(Timer&& timer);
};

} // namespace Profiling