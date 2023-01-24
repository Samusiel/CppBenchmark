#pragma once

#include <profiling/ProfileMark.hpp>

namespace Profiling {

using TimeDuration = std::chrono::nanoseconds;

struct Timer {
    ProfileMark mark;
    TimeDuration duration;
};

struct Counter {
    ProfileMark mark;
    size_t count;
};

class IProfileOutput {
public:
    virtual ~IProfileOutput() = default;

    virtual void emit(Timer timer) = 0;
    virtual void emit(Counter counter) = 0;
};

} // namespace Profiling