#include <iostream>
#include <chrono>
#include "../../include/my_stl/pair.hpp"
#include <utility>

// 测试数据结构
struct LargeData {
    int data[64];
    LargeData() { for (int i = 0; i < 64; ++i) data[i] = i; }
    LargeData(const LargeData& other) { for (int i = 0; i < 64; ++i) data[i] = other.data[i]; }
    LargeData(LargeData&& other) noexcept { for (int i = 0; i < 64; ++i) data[i] = other.data[i]; }
    LargeData& operator=(const LargeData& other) { 
        for (int i = 0; i < 64; ++i) data[i] = other.data[i];
        return *this;
    }
    LargeData& operator=(LargeData&& other) noexcept {
        for (int i = 0; i < 64; ++i) data[i] = other.data[i];
        return *this;
    }
};

// 简单的基准测试函数
template<typename Func>
double benchmark(Func&& func, int iterations = 1000000) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    return static_cast<double>(duration.count()) / iterations;
}

int main() {
    std::cout << "=== Assignment Performance Test ===" << std::endl;
    std::cout << "Iterations: 1,000,000" << std::endl << std::endl;
    
    // 测试赋值操作
    double mystl_assign = benchmark([]() {
        my_stl::pair<LargeData, LargeData> p1, p2;
        p1 = p2;
        asm volatile("" : : "r,m"(p1) : "memory");
    });
    
    double std_assign = benchmark([]() {
        std::pair<LargeData, LargeData> p1, p2;
        p1 = p2;
        asm volatile("" : : "r,m"(p1) : "memory");
    });
    
    std::cout << "=== Assignment Results ===" << std::endl;
    std::cout << "my_stl::pair: " << mystl_assign << " ns/op" << std::endl;
    std::cout << "std::pair:    " << std_assign << " ns/op" << std::endl;
    std::cout << "Ratio (my_stl/std): " << (mystl_assign / std_assign) << "x" << std::endl;
    std::cout << std::endl;
    
    // 测试移动赋值操作
    double mystl_move_assign = benchmark([]() {
        my_stl::pair<LargeData, LargeData> p1, p2;
        p1 = std::move(p2);
        asm volatile("" : : "r,m"(p1) : "memory");
    });
    
    double std_move_assign = benchmark([]() {
        std::pair<LargeData, LargeData> p1, p2;
        p1 = std::move(p2);
        asm volatile("" : : "r,m"(p1) : "memory");
    });
    
    std::cout << "=== Move Assignment Results ===" << std::endl;
    std::cout << "my_stl::pair: " << mystl_move_assign << " ns/op" << std::endl;
    std::cout << "std::pair:    " << std_move_assign << " ns/op" << std::endl;
    std::cout << "Ratio (my_stl/std): " << (mystl_move_assign / std_move_assign) << "x" << std::endl;
    std::cout << std::endl;
    
    // 测试小对象赋值
    double mystl_small_assign = benchmark([]() {
        my_stl::pair<int, int> p1, p2;
        p1 = p2;
        asm volatile("" : : "r,m"(p1) : "memory");
    });
    
    double std_small_assign = benchmark([]() {
        std::pair<int, int> p1, p2;
        p1 = p2;
        asm volatile("" : : "r,m"(p1) : "memory");
    });
    
    std::cout << "=== Small Object Assignment Results ===" << std::endl;
    std::cout << "my_stl::pair: " << mystl_small_assign << " ns/op" << std::endl;
    std::cout << "std::pair:    " << std_small_assign << " ns/op" << std::endl;
    std::cout << "Ratio (my_stl/std): " << (mystl_small_assign / std_small_assign) << "x" << std::endl;
    
    return 0;
}