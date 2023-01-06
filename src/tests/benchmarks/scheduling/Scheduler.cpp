#include <benchmark/benchmark.h>
#include <scheduling/ScheduleManager.hpp>
#include <scheduling/Scheduler.hpp>

namespace Scheduling { } // namespace Scheduling

static void BM_Scheduler(benchmark::State& state) {
    // Perform setup here
    auto threads = uint8_t(state.range(0));
    auto tasks = state.range(1);
    auto scheduleManager = Scheduling::ScheduleManager{4};

    for (auto _ : state) {
        auto s = scheduleManager.create(threads);
        // This code gets timed
        for (int i = 0; i < tasks; ++i) {
            s.schedule([] {});
        }
    }
}

// Register the function as a benchmark
BENCHMARK(BM_Scheduler)
    ->Args({1, 8192})
    ->Args({1, 1000000})
    ->Args({4, 8192})
    ->Args({4, 1000000})
    ->Args({8, 8192})
    ->Args({8, 1000000});

// Run the benchmark
BENCHMARK_MAIN();