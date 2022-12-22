#include <benchmark/benchmark.h>
#include <config/ConfigRegistry.hpp>

namespace {
    ConfigLibrary::ConfigRegistry createRegistry() {
        ConfigLibrary::ConfigRegistry registry;
        return registry;
    }

    void configVariableAccess() {
        static auto registry = createRegistry();

        
    }
}

static void BM_SomeFunction(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    SomeFunction();
  }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();