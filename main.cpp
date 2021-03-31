#include <chrono>
#include <iostream>
#include <memory>
#include <list>
#include <numeric>
#include <random>
#include <iomanip>
#include "include/map.h"

enum WorkloadDistribution { UNIFORM, NORMAl };

auto CreateWorkload(WorkloadDistribution workload_distribution, size_t max_index, size_t workload_size) {
  std::vector<int> workload_data;
  workload_data.resize(workload_size);

  std::random_device rd;
  std::mt19937 gen(rd());

  switch (workload_distribution) {
    case UNIFORM: {
      std::uniform_int_distribution<> distrib(0, max_index - 1);
      for (auto &k: workload_data) k = distrib(gen);
      break;
    }
    case NORMAl: {
      std::normal_distribution<> distrib(max_index * 0.5, 500);
      for (auto &k: workload_data) k = (int) distrib(gen) % max_index;
      break;
    }
  }
  return workload_data;
}

template<typename F, typename ...Args>
size_t TimedExecution(F f, Args &&... args) {
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
  f(args...);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  std::cout << "took " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]\n";
  std::cout << std::endl;
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
}

int main() {
  const size_t kN = 10'000'000;
  const size_t kWorkloadSize = 10'000'000;
  Map<int, int> *vector_map = new VectorMap<int, int>();

  std::vector<int> indexed_data(kN);
  std::iota(indexed_data.begin(), indexed_data.end(), 0);

  std::vector<std::pair<int, int>> plain_vector;

  for (auto k : indexed_data) {
    vector_map->Add(k, k);
    plain_vector.emplace_back(k, k);
  }

  std::shuffle(indexed_data.begin(), indexed_data.end(), std::mt19937(std::random_device()()));

  auto benchmark_type_erasure = [&](std::vector<int> &benchmark_data) {
    size_t fingerprint = 0;
    for (auto k : benchmark_data) {
      auto it = vector_map->Find(k);
      fingerprint += it->second;
    }
    std::cout << "hash=" << fingerprint << "\n";
  };

  auto benchmark_direct = [&](std::vector<int> &benchmark_data) {
    size_t fingerprint = 0;
    for (auto k : benchmark_data) {
      auto it = std::lower_bound(plain_vector.begin(),
                                 plain_vector.end(),
                                 k,
                                 [](const std::pair<int, int> &a, int b) { return a.second < b; });
      fingerprint += it->second;
    }
    std::cout << "hash=" << fingerprint << "\n";
  };

  std::vector<int> benchmark_data;

  std::cout << "run benchmark with uniform data..." << std::endl;
  benchmark_data = CreateWorkload(UNIFORM, kN, kWorkloadSize);
  auto time_type_erasure = TimedExecution(benchmark_type_erasure, benchmark_data);
  auto time_direct = TimedExecution(benchmark_direct, benchmark_data);

  std::cout << std::setprecision(3);
  std::cout << "overhead with type erasure: " << time_type_erasure * 100 / time_direct - 100 << "%" << std::endl;

  std::cout << "run benchmark with uniform data..." << std::endl;
  benchmark_data = CreateWorkload(NORMAl, kN, kWorkloadSize);
  time_type_erasure = TimedExecution(benchmark_type_erasure, benchmark_data);
  time_direct = TimedExecution(benchmark_direct, benchmark_data);

  std::cout << std::setprecision(3);
  std::cout << "overhead with type erasure: " << time_type_erasure * 100 / time_direct - 100 << "%" << std::endl;
  return 0;
}
