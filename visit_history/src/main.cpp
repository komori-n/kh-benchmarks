#include <benchmark/benchmark.h>
#include <iostream>

#include "open_address.hpp"
#include "unordered_multimap.hpp"

namespace {
template <typename VisitHistory>
void InsertAndErase(benchmark::State& state) {
  VisitHistory history{};
  for (std::uint32_t i = 0; i < 1000; ++i) {
    history.Visit(334, i, i);
  }

  for (auto _ : state) {
    history.Visit(334, 10000, 10000);
    history.Leave(334, 10000);
  }

  for (std::uint32_t i = 0; i < 1000; ++i) {
    history.Leave(334, i);
  }
}

BENCHMARK(InsertAndErase<komori::old::VisitHistory>);
BENCHMARK(InsertAndErase<komori::open_addressing::VisitHistory<32768>>);

template <typename VisitHistory>
void InsertLookupErase(benchmark::State& state) {
  VisitHistory history{};
  for (std::uint32_t i = 0; i < 1000; ++i) {
    history.Visit(334, i, i);
  }

  for (auto _ : state) {
    history.Visit(334, 10000, 10000);
    benchmark::DoNotOptimize(history.Contains(334, 10000));
    benchmark::DoNotOptimize(history.Contains(335, 10000));
    benchmark::DoNotOptimize(history.Contains(336, 10000));
    history.Leave(334, 10000);
  }

  for (std::uint32_t i = 0; i < 1000; ++i) {
    history.Leave(334, i);
  }
}

BENCHMARK(InsertLookupErase<komori::old::VisitHistory>);
BENCHMARK(InsertLookupErase<komori::open_addressing::VisitHistory<32768>>);
}  // namespace

BENCHMARK_MAIN();
