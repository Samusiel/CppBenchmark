#include <benchmark/benchmark.h>
#include <config/ConfigRegistry.hpp>

static void BM_GetVariableValue(benchmark::State& state) {
    static ConfigLibrary::ConfigRegistry registry;
    // Perform setup here
    auto size = state.range(0);
    for (auto _ : state) {
        // This code gets timed
        for (int i = 0; i < size; ++i) {
            [[maybe_unused]] int value = registry.exampleVariable.getValue();
        }
    }
}

static void BM_GetVariableValueByName(benchmark::State& state) {
    static ConfigLibrary::ConfigRegistry registry;
    auto size = state.range(0);
    // Perform setup here
    for (auto _ : state) {
        // This code gets timed
        for (int i = 0; i < size; ++i) {
            [[maybe_unused]] int value = registry.getConfigVariableValueByName<int>("Hello World");
        }
    }
}

// Register the function as a benchmark
constexpr int NumberOfIterations = 1024;
BENCHMARK(BM_GetVariableValue)->Arg(NumberOfIterations);
BENCHMARK(BM_GetVariableValueByName)->Arg(NumberOfIterations);

// Run the benchmark
BENCHMARK_MAIN();