#pragma once

#include <profiling/ProfileBackend.hpp>
#include <profiling/ProfileMark.hpp>
#include <profiling/Profiler.hpp>
#include <string_view>
#include <variant>

namespace Profiling {

template <typename Backend>
concept ProfileBackendConcept = requires(Backend&& backend) {
                                    { backend.begin() } -> std::same_as<ProfilingTimePoint>;
                                    { backend.end() } -> std::same_as<ProfilingTimePoint>;
                                    { backend.submit(Profiler::instance(), TimeDuration{}) } -> std::same_as<void>;
                                };

template <ProfileBackendConcept Backend = CPUBackend>
class ProfileScopeGuard {
public:
    // ToDo: try to make it constexpr
    ProfileScopeGuard(const Backend& backend)
        : _profiler(Profiler::instance())
        , _backend(backend) {
        std::visit(
            [this](const auto& backend) constexpr {
                _begin = backend.begin();
            },
            _backend);
    }

    constexpr ProfileScopeGuard(ProfileScopeGuard&& guard)
        : _profiler(guard._profiler)
        , _backend(std::move(guard._backend))
        , _begin(std::move(guard._begin))
        , _end(std::move(guard._end)) {
        guard._backend = ProfileBackendFactory::createMockBackend();
    }

    constexpr ~ProfileScopeGuard() {
        std::visit(
            [this](const auto& backend) {
                _end = backend.end();
                backend.submit(_profiler, _end - _begin);
            },
            _backend);
    }

private:
    Profiler& _profiler;
    std::variant<Backend, MockBackend> _backend;
    ProfilingTimePoint _begin;
    ProfilingTimePoint _end;
};

} // namespace Profiling