#include <iostream>
#include <chrono>
#include <vector>
#include <utility>
#include <random>
#include <algorithm>
#include <memory>
#include <functional>
#include "../../include/my_stl/pair.hpp"

// Benchmark utilities
class Benchmark {
public:
    static double measure_time(std::function<void()> func, int iterations = 1) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            func();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        return duration.count() / static_cast<double>(iterations);
    }
    
    static void compare_performance(const std::string& test_name,
                                  std::function<void()> my_stl_func,
                                  std::function<void()> std_func,
                                  int iterations = 1000000) {
        std::cout << "\n=== " << test_name << " ===" << std::endl;
        
        double my_time = measure_time(my_stl_func, iterations);
        double std_time = measure_time(std_func, iterations);
        
        std::cout << "my_stl::pair: " << my_time << " ns/op" << std::endl;
        std::cout << "std::pair:    " << std_time << " ns/op" << std::endl;
        
        double ratio = my_time / std_time;
        std::cout << "Ratio (my_stl/std): " << ratio << "x";
        if (ratio < 1.0) {
            std::cout << " (faster)";
        } else if (ratio > 1.0) {
            std::cout << " (slower)";
        } else {
            std::cout << " (same)";
        }
        std::cout << std::endl;
    }
};

// Test data
struct LargeStruct {
    int data[64];
    LargeStruct() { std::fill(data, data + 64, 42); }
    LargeStruct(int val) { std::fill(data, data + 64, val); }
};

struct ExpensiveToCopy {
    std::vector<int> data;
    ExpensiveToCopy() : data(1000, 42) {}
    ExpensiveToCopy(int size) : data(size, 42) {}
};

// Benchmark: Basic construction
void benchmark_construction() {
    volatile int sink = 0;
    
    auto my_stl_test = [&sink]() {
        my_stl::pair<int, int> p(42, 84);
        sink += p.first;
    };
    
    auto std_test = [&sink]() {
        std::pair<int, int> p(42, 84);
        sink += p.first;
    };
    
    Benchmark::compare_performance("Basic Construction", my_stl_test, std_test);
}

// Benchmark: Copy construction
void benchmark_copy_construction() {
    my_stl::pair<int, int> my_source(42, 84);
    std::pair<int, int> std_source(42, 84);
    volatile int sink = 0;
    
    auto my_stl_test = [&]() {
        my_stl::pair<int, int> p = my_source;
        sink += p.first;
    };
    
    auto std_test = [&]() {
        std::pair<int, int> p = std_source;
        sink += p.first;
    };
    
    Benchmark::compare_performance("Copy Construction", my_stl_test, std_test);
}

// Benchmark: Move construction
void benchmark_move_construction() {
    volatile int sink = 0;
    
    auto my_stl_test = [&sink]() {
        my_stl::pair<LargeStruct, LargeStruct> p1(LargeStruct(1), LargeStruct(2));
        my_stl::pair<LargeStruct, LargeStruct> p2 = std::move(p1);
        sink += p2.first.data[0];
    };
    
    auto std_test = [&sink]() {
        std::pair<LargeStruct, LargeStruct> p1(LargeStruct(1), LargeStruct(2));
        std::pair<LargeStruct, LargeStruct> p2 = std::move(p1);
        sink += p2.first.data[0];
    };
    
    Benchmark::compare_performance("Move Construction", my_stl_test, std_test, 100000);
}

// Benchmark: Assignment
void benchmark_assignment() {
    my_stl::pair<int, int> my_p1(1, 2);
    my_stl::pair<int, int> my_p2(3, 4);
    std::pair<int, int> std_p1(1, 2);
    std::pair<int, int> std_p2(3, 4);
    volatile int sink = 0;
    
    auto my_stl_test = [&]() {
        my_p1 = my_p2;
        sink += my_p1.first;
    };
    
    auto std_test = [&]() {
        std_p1 = std_p2;
        sink += std_p1.first;
    };
    
    Benchmark::compare_performance("Assignment", my_stl_test, std_test);
}

// Benchmark: make_pair
void benchmark_make_pair() {
    volatile int sink = 0;
    
    auto my_stl_test = [&sink]() {
        auto p = my_stl::make_pair(42, 84);
        sink += p.first;
    };
    
    auto std_test = [&sink]() {
        auto p = std::make_pair(42, 84);
        sink += p.first;
    };
    
    Benchmark::compare_performance("make_pair", my_stl_test, std_test);
}

// Benchmark: Swap
void benchmark_swap() {
    my_stl::pair<int, int> my_p1(1, 2);
    my_stl::pair<int, int> my_p2(3, 4);
    std::pair<int, int> std_p1(1, 2);
    std::pair<int, int> std_p2(3, 4);
    volatile int sink = 0;
    
    auto my_stl_test = [&]() {
        my_p1.swap(my_p2);
        sink += my_p1.first;
    };
    
    auto std_test = [&]() {
        std_p1.swap(std_p2);
        sink += std_p1.first;
    };
    
    Benchmark::compare_performance("Swap", my_stl_test, std_test);
}

// Benchmark: Comparison operations
void benchmark_comparison() {
    my_stl::pair<int, int> my_p1(1, 2);
    my_stl::pair<int, int> my_p2(3, 4);
    std::pair<int, int> std_p1(1, 2);
    std::pair<int, int> std_p2(3, 4);
    volatile bool sink = false;
    
    auto my_stl_test = [&]() {
        sink = (my_p1 < my_p2) && (my_p1 == my_p1) && (my_p1 != my_p2);
    };
    
    auto std_test = [&]() {
        sink = (std_p1 < std_p2) && (std_p1 == std_p1) && (std_p1 != std_p2);
    };
    
    Benchmark::compare_performance("Comparison", my_stl_test, std_test);
}

// Benchmark: Container operations
void benchmark_container_operations() {
    const int size = 10000;
    volatile int sink = 0;
    
    auto my_stl_test = [&sink, size]() {
        std::vector<my_stl::pair<int, int>> vec;
        vec.reserve(size);
        for (int i = 0; i < size; ++i) {
            vec.emplace_back(i, i * 2);
        }
        for (const auto& p : vec) {
            sink += p.first;
        }
    };
    
    auto std_test = [&sink, size]() {
        std::vector<std::pair<int, int>> vec;
        vec.reserve(size);
        for (int i = 0; i < size; ++i) {
            vec.emplace_back(i, i * 2);
        }
        for (const auto& p : vec) {
            sink += p.first;
        }
    };
    
    Benchmark::compare_performance("Container Operations", my_stl_test, std_test, 1000);
}

// Benchmark: Memory access patterns
void benchmark_memory_access() {
    const int size = 100000;
    std::vector<my_stl::pair<int, int>> my_vec(size);
    std::vector<std::pair<int, int>> std_vec(size);
    
    // Initialize with random data
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    
    for (int i = 0; i < size; ++i) {
        int val1 = dis(gen);
        int val2 = dis(gen);
        my_vec[i] = my_stl::make_pair(val1, val2);
        std_vec[i] = std::make_pair(val1, val2);
    }
    
    volatile long long sink = 0;
    
    auto my_stl_test = [&]() {
        for (const auto& p : my_vec) {
            sink += p.first + p.second;
        }
    };
    
    auto std_test = [&]() {
        for (const auto& p : std_vec) {
            sink += p.first + p.second;
        }
    };
    
    Benchmark::compare_performance("Memory Access", my_stl_test, std_test, 100);
}

// Memory usage comparison
void benchmark_memory_usage() {
    std::cout << "\n=== Memory Usage Comparison ===" << std::endl;
    
    std::cout << "Basic types:" << std::endl;
    std::cout << "  my_stl::pair<int, int>: " << sizeof(my_stl::pair<int, int>) << " bytes" << std::endl;
    std::cout << "  std::pair<int, int>:    " << sizeof(std::pair<int, int>) << " bytes" << std::endl;
    
    std::cout << "  my_stl::pair<char, char>: " << sizeof(my_stl::pair<char, char>) << " bytes" << std::endl;
    std::cout << "  std::pair<char, char>:    " << sizeof(std::pair<char, char>) << " bytes" << std::endl;
    
    std::cout << "  my_stl::pair<double, double>: " << sizeof(my_stl::pair<double, double>) << " bytes" << std::endl;
    std::cout << "  std::pair<double, double>:    " << sizeof(std::pair<double, double>) << " bytes" << std::endl;
    
    // EBCO test
    struct Empty {};
    std::cout << "\nEBCO effectiveness:" << std::endl;
    std::cout << "  my_stl::pair<Empty, int>: " << sizeof(my_stl::pair<Empty, int>) << " bytes" << std::endl;
    std::cout << "  std::pair<Empty, int>:    " << sizeof(std::pair<Empty, int>) << " bytes" << std::endl;
    
    std::cout << "  my_stl::pair<Empty, Empty>: " << sizeof(my_stl::pair<Empty, Empty>) << " bytes" << std::endl;
    std::cout << "  std::pair<Empty, Empty>:    " << sizeof(std::pair<Empty, Empty>) << " bytes" << std::endl;
}

int main() {
    std::cout << "=== my_stl::pair Comprehensive Benchmarks ===" << std::endl;
    std::cout << "Running performance comparisons between my_stl::pair and std::pair..." << std::endl;
    
    try {
        benchmark_construction();
        benchmark_copy_construction();
        benchmark_move_construction();
        benchmark_assignment();
        benchmark_make_pair();
        benchmark_swap();
        benchmark_comparison();
        benchmark_container_operations();
        benchmark_memory_access();
        benchmark_memory_usage();
        
        std::cout << "\n✅ All benchmarks completed!" << std::endl;
        std::cout << "\nNote: Performance results may vary depending on compiler optimizations and system load." << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ Benchmark failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}