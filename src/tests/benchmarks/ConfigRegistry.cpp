#include <benchmark/benchmark.h>
#include <config/ConfigRegistry.hpp>

namespace {
    ConfigLibrary::ConfigRegistry createRegistry() {
        ConfigLibrary::ConfigRegistry registry;
        return registry;
    }

    void configVariableAccess() {
        static auto registry = createRegistry();
        auto temp = registry.registerConfigVariable<int>("Hello World", 42);
        
    }
}

static void BM_SomeFunction(benchmark::State& state) {
  // Perform setup here
  for (auto _ : state) {
    // This code gets timed
    configVariableAccess();
  }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();