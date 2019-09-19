//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <algorithm>
#include <list>
#include <string>
#include <utility>
#include <vector>

#include "benchmark/benchmark.h"
#include "GenerateInput.h"

struct R {
  std::vector<char> m;
  void initializeWith(const std::string& s) {
    m.assign(s.begin(), s.end());
  }
  struct Less {
    bool operator()(const R& a, const R& b) const noexcept {
        return a.m < b.m;
    }
  };
};

struct NR {
  std::list<char> m;
  void initializeWith(const std::string& s) {
    m.assign(s.begin(), s.end());
  }
  struct Less {
    bool operator()(const NR& a, const NR& b) const noexcept {
      return a.m < b.m;
    }
  };
};

static void BM_TriviallyRelocatableSwapRanges(benchmark::State& st) {
  std::vector<R> pv(1000);
  std::vector<R> qv(1000);
  while (st.KeepRunning()) {
    std::swap_ranges(pv.begin(), pv.end(), qv.begin());
    benchmark::DoNotOptimize(pv);
    benchmark::DoNotOptimize(qv);
  }
}
BENCHMARK(BM_TriviallyRelocatableSwapRanges);

static void BM_NonTriviallyRelocatableSwapRanges(benchmark::State& st) {
  std::vector<NR> pv(1000);
  std::vector<NR> qv(1000);
  while (st.KeepRunning()) {
    std::swap_ranges(pv.begin(), pv.end(), qv.begin());
    benchmark::DoNotOptimize(pv);
    benchmark::DoNotOptimize(qv);
  }
}
BENCHMARK(BM_NonTriviallyRelocatableSwapRanges);

static void BM_TriviallyRelocatableMakeHeap(benchmark::State& st) {
  std::vector<R> original(1000);
  for (R& elt : original)
    elt.initializeWith(getRandomString(256));
  std::vector<R> v(1000);
  while (st.KeepRunning()) {
    st.PauseTiming();
    v = original;
    st.ResumeTiming();
    std::make_heap(v.begin(), v.end(), R::Less{});
    benchmark::DoNotOptimize(v);
  }
}
BENCHMARK(BM_TriviallyRelocatableMakeHeap);

static void BM_NonTriviallyRelocatableMakeHeap(benchmark::State& st) {
  std::vector<NR> original(1000);
  for (NR& elt : original)
    elt.initializeWith(getRandomString(256));
  std::vector<NR> v(1000);
  while (st.KeepRunning()) {
    st.PauseTiming();
    v = original;
    st.ResumeTiming();
    std::make_heap(v.begin(), v.end(), NR::Less{});
    benchmark::DoNotOptimize(v);
  }
}
BENCHMARK(BM_NonTriviallyRelocatableMakeHeap);

BENCHMARK_MAIN();
