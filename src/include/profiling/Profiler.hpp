#pragma once

#include <profiling/IProfileOutput.hpp>
#include <profiling/ProfileSettings.hpp>
#include <vector>

namespace Profiling {

class Profiler {
public:
    // ToDo: remove this method and put profiler under the App class
    static Profiler& instance() noexcept {
        static Profiler profiler{ProfileSettings{}};
        return profiler;
    }

    Profiler(ProfileSettings&& settings);

    void emit(Timer&& timer);
    void emit(Counter&& counter);

private:
    ProfileSettings _settings;
    std::vector<std::unique_ptr<IProfileOutput>> _outputs;
};

} // namespace Profiling