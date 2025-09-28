#include <iostream>
#include <chrono>
#include <vector>
#include <utility>
#include <random>
#include <algorithm>
#include <memory>
#include <functional>
#include "../../include/my_stl/pair.hpp"

// 优化的基准测试工具
class OptimizedBenchmark {
public:
    static double measure_time(std::function<void()> func, int warmup = 1000, int iterations = 10000) {
        // 预热阶段，避免冷启动影响
        for (int i = 0; i < warmup; ++i) {
            func();
        }
        
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
                                  int warmup = 1000, 
                                  int iterations = 100000) {
        std::cout << "\n=== " << test_name << " ===" << std::endl;
        
        // 多次测量取平均值
        const int runs = 5;
        double my_time_total = 0.0;
        double std_time_total = 0.0;
        
        for (int i = 0; i < runs; ++i) {
            my_time_total += measure_time(my_stl_func, warmup, iterations);
            std_time_total += measure_time(std_func, warmup, iterations);
        }
        
        double my_time = my_time_total / runs;
        double std_time = std_time_total / runs;
        
        std::cout << "my_stl::pair: " << my_time << " ns/op" << std::endl;
        std::cout << "std::pair:    " << std_time << " ns/op" << std::endl;
        
        double ratio = my_time / std_time;
        std::cout << "Ratio (my_stl/std): " << ratio << "x";
        if (ratio < 0.95) {
            std::cout << " (faster)" << std::endl;
        } else if (ratio > 1.05) {
            std::cout << " (slower)" << std::endl;
        } else {
            std::cout << " (similar)" << std::endl;
        }
    }
};

// 测试数据结构
struct LargeData {
    int data[128];
    LargeData() { std::fill(data, data + 128, 42); }
    LargeData(int val) { std::fill(data, data + 128, val); }
    
    // 确保移动语义正确
    LargeData(LargeData&& other) noexcept {
        std::copy(other.data, other.data + 128, data);
    }
    
    LargeData& operator=(LargeData&& other) noexcept {
        if (this != &other) {
            std::copy(other.data, other.data + 128, data);
        }
        return *this;
    }
};

// 专门测试移动构造性能
void benchmark_move_construction_optimized() {
    volatile int sink = 0;
    
    auto my_stl_test = [&sink]() {
        my_stl::pair<LargeData, LargeData> p1(LargeData(1), LargeData(2));
        my_stl::pair<LargeData, LargeData> p2 = std::move(p1);
        sink += p2.first.data[0];
    };
    
    auto std_test = [&sink]() {
        std::pair<LargeData, LargeData> p1(LargeData(1), LargeData(2));
        std::pair<LargeData, LargeData> p2 = std::move(p1);
        sink += p2.first.data[0];
    };
    
    OptimizedBenchmark::compare_performance("Move Construction (Optimized)", 
                                          my_stl_test, std_test, 1000, 50000);
}

// 专门测试赋值性能
void benchmark_assignment_optimized() {
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
    
    OptimizedBenchmark::compare_performance("Assignment (Optimized)", 
                                          my_stl_test, std_test, 1000, 100000);
}

// 测试小对象性能
void benchmark_small_objects() {
    volatile int sink = 0;
    
    auto my_stl_test = [&sink]() {
        my_stl::pair<int, int> p1(1, 2);
        my_stl::pair<int, int> p2 = std::move(p1);
        sink += p2.first;
    };
    
    auto std_test = [&sink]() {
        std::pair<int, int> p1(1, 2);
        std::pair<int, int> p2 = std::move(p1);
        sink += p2.first;
    };
    
    OptimizedBenchmark::compare_performance("Small Objects Move", 
                                          my_stl_test, std_test, 1000, 200000);
}

// 测试noexcept优化效果
void benchmark_noexcept_optimization() {
    struct NoexceptMove {
        int data[64];
        NoexceptMove() noexcept {}
        NoexceptMove(NoexceptMove&&) noexcept = default;
        NoexceptMove& operator=(NoexceptMove&&) noexcept = default;
    };
    
    volatile int sink = 0;
    
    auto my_stl_test = [&sink]() {
        my_stl::pair<NoexceptMove, NoexceptMove> p1;
        my_stl::pair<NoexceptMove, NoexceptMove> p2 = std::move(p1);
        sink += p2.first.data[0];
    };
    
    auto std_test = [&sink]() {
        std::pair<NoexceptMove, NoexceptMove> p1;
        std::pair<NoexceptMove, NoexceptMove> p2 = std::move(p1);
        sink += p2.first.data[0];
    };
    
    OptimizedBenchmark::compare_performance("Noexcept Optimization", 
                                          my_stl_test, std_test, 1000, 100000);
}

// 测试容器操作性能
void benchmark_container_performance() {
    const int size = 1000;
    volatile int sink = 0;
    
    auto my_stl_test = [&sink, size]() {
        std::vector<my_stl::pair<int, int>> vec;
        vec.reserve(size);
        for (int i = 0; i < size; ++i) {
            vec.emplace_back(i, i * 2);
        }
        // 测试移动语义在容器中的性能
        std::vector<my_stl::pair<int, int>> vec2 = std::move(vec);
        sink += vec2.size();
    };
    
    auto std_test = [&sink, size]() {
        std::vector<std::pair<int, int>> vec;
        vec.reserve(size);
        for (int i = 0; i < size; ++i) {
            vec.emplace_back(i, i * 2);
        }
        std::vector<std::pair<int, int>> vec2 = std::move(vec);
        sink += vec2.size();
    };
    
    OptimizedBenchmark::compare_performance("Container Performance", 
                                          my_stl_test, std_test, 100, 1000);
}

int main() {
    std::cout << "=== Optimized my_stl::pair Performance Benchmarks ===" << std::endl;
    std::cout << "Testing performance with optimized implementations..." << std::endl;
    
    try {
        benchmark_move_construction_optimized();
        benchmark_assignment_optimized();
        benchmark_small_objects();
        benchmark_noexcept_optimization();
        benchmark_container_performance();
        
        std::cout << "\n✅ All optimized benchmarks completed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "\n❌ Benchmark failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}