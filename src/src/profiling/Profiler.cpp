#include <profiling/ProfileOutputFactory.hpp>
#include <profiling/Profiler.hpp>

namespace Profiling {

Profiler::Profiler(ProfileSettings&& settings) {
    for (auto& outputSettings : settings.outputs) {
        std::visit(
            [&](auto& settings) {
                ProfileOutputFactory::create(settings);
            },
            outputSettings);
    }
}

void Profiler::emit(Timer&& timer) {
    for (const auto& output : _outputs) {
        output->emit(timer);
    }
}

void Profiler::emit(Counter&& counter) {
    for (const auto& output : _outputs) {
        output->emit(counter);
    }
}

} // namespace Profiling