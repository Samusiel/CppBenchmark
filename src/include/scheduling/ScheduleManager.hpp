#pragma once

#include <scheduling/Scheduler.hpp>
#include <scheduling/Worker.hpp>

namespace Scheduling {

class ScheduleManager {
private:
    constexpr static uint8_t MaxPoolSize = 8;

public:
    ScheduleManager(uint8_t poolSize) noexcept
        : _pool(
            [](Scheduler* scheduler, uint8_t threads) {
                assert(threads <= MaxPoolSize);
                return std::make_unique<Worker>(scheduler, threads);
            },
            poolSize) { }

    auto create(uint8_t threads) noexcept {
        return Scheduler{[](Scheduler* scheduler, uint8_t threads) {
                             return std::make_unique<Worker>(scheduler, threads);
                         },
            threads};
    }

    template <WorkerConcept WorkerType = Worker>
    auto create() noexcept {
        return Scheduler{[](Scheduler* scheduler, uint8_t threads) {
                             return std::make_unique<WorkerType>(scheduler, threads);
                         },
            1};
    }

    auto& pool() noexcept { return _pool; }

private:
    Scheduler _pool;
};

} // namespace Scheduling