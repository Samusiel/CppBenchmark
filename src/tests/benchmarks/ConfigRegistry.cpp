#include <benchmark/benchmark.h>
#include <config/ConfigRegistry.hpp>

namespace {
    ConfigLibrary::ConfigRegistry createRegistry() {
        ConfigLibrary::ConfigRegistry registry;
        return registry;
    }

    void configVariableAccess() {
        
        
    }
}

static void BM_SomeFunction(benchmark::State& state) {
  static auto registry = createRegistry();
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    for (int i = 0; i < 1024; ++i) {
      [[maybe_unused]]
      int value = registry.exampleVariable.getValue();
    }
  }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();