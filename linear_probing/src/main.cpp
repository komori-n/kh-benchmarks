#include <benchmark/benchmark.h>

#include <random>
#include "linear_probing.hpp"

namespace {
using Key = std::uint64_t;
using Value = std::uint64_t;

std::vector<std::pair<Key, Value>> CreateRandomQueries(std::size_t n) {
  std::vector<std::pair<Key, Value>> vec;
  vec.reserve(n);

  std::mt19937 mt{334};
  std::uniform_int_distribution<Key> key_dist;
  std::uniform_int_distribution<Value> value_dist;
  for (std::size_t i = 0; i < n; ++i) {
    const auto key = key_dist(mt);
    const auto value = value_dist(mt);

    vec.emplace_back(key, value);
  }

  return vec;
}

template <std::size_t N>
void LinearProbingBenchmark(benchmark::State& state) {
  komori::LinearProbingTable<Key, Value, N> table{};

  const std::size_t K1 = state.range(0);
  const std::size_t K2 = state.range(1);
  const std::size_t a = state.range(2);

  const auto queries = CreateRandomQueries(10 * N);
  std::size_t i = 0;

  const auto insert = [&]() {
    const auto [key, value] = queries[i];
    i = (i + 1) % queries.size();
    table.Insert(key, value);
  };
  const auto lookup = [&]() {
    const auto [key, value] = queries[i];
    i = (i + 1) % queries.size();
    benchmark::DoNotOptimize(table.LookUp(key));
  };

  for (std::size_t j = 0; j < K1; ++j) {
    insert();
  }

  for (auto _ : state) {
    for (std::size_t j = K1; j < K2; ++j) {
      insert();
      for (std::size_t l = 0; l < a - 1; ++l) {
        lookup();
      }
    }
    table.CollectGarbage(K2 - K1);
  }
}

template <std::size_t N>
void CustomArguments(benchmark::internal::Benchmark* b) {
  const auto step = static_cast<int>(N * 0.01);
  const auto k1 = static_cast<int>(N * 0.3);
  const int a = 5;

  for (int k2 = k1 + step; k2 < static_cast<int>(N); k2 += step) {
    b->Args({k1, k2, a});
  }
}

BENCHMARK(LinearProbingBenchmark<4096>)->Apply(CustomArguments<4096>);
}  // namespace

BENCHMARK_MAIN();