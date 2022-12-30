#include <benchmark/benchmark.h>
#include <scheduling/Scheduler.hpp>

namespace Scheduling {

class BenchmarkScheduler : public Scheduler {
public:
    BenchmarkScheduler(bool start_, uint8_t threads_): Scheduler(start_, threads_), _start(start_), _threads(threads_) {}
    ~BenchmarkScheduler() {
        if (!_start) {
            start(_threads);
        }
    }
    void testStart(uint8_t threads_) { start(threads_); }

private:
    bool _start = false;
    uint8_t _threads = 1;
};

} // namespace Scheduling

static void BM_Scheduler(benchmark::State& state) {
  // Perform setup here
  auto immediateStart = bool(state.range(0));
  auto threads = uint8_t(state.range(1));
  auto tasks = state.range(2);
  
  for (auto _ : state) {
    Scheduling::BenchmarkScheduler s{immediateStart, threads};
    // This code gets timed
    for (int i = 0; i < tasks; ++i) {
      s.schedule([] {});
    }
  }
}

// Register the function as a benchmark
BENCHMARK(BM_Scheduler)
    ->Args({false, 1, 8192})
    ->Args({false, 1, 1000000})
    ->Args({false, 4, 8192})
    ->Args({false, 4, 1000000})
    ->Args({false, 8, 8192})
    ->Args({false, 8, 1000000})
    ->Args({true, 1, 8192})
    ->Args({true, 1, 1000000})
    ->Args({true, 4, 8192})
    ->Args({true, 4, 1000000})
    ->Args({true, 8, 8192})
    ->Args({true, 8, 1000000});

// Run the benchmark
BENCHMARK_MAIN();