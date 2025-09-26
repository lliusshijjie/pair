#include <iostream>
#include <cassert>
#include <chrono>
#include <vector>
#include <utility>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include "../../include/my_stl/pair.hpp"

// Set console encoding to UTF-8
void setup_console_encoding() {
#ifdef _WIN32
    // Set console code page to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // Set standard output to UTF-8 mode
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);
#endif
}

// Performance test utilities
class Timer {
public:
    Timer() : start_(std::chrono::high_resolution_clock::now()) {}
    
    double elapsed() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
        return duration.count() / 1000.0; // Return milliseconds
    }
    
private:
    std::chrono::high_resolution_clock::time_point start_;
};

// Test data structures
struct LargeObject {
    int data[100];
    LargeObject() { std::fill(data, data + 100, 42); }
    LargeObject(int val) { std::fill(data, data + 100, val); }
};

struct MovableOnly {
    std::unique_ptr<int> ptr;
    MovableOnly() : ptr(std::make_unique<int>(42)) {}
    MovableOnly(int val) : ptr(std::make_unique<int>(val)) {}
    MovableOnly(const MovableOnly&) = delete;
    MovableOnly& operator=(const MovableOnly&) = delete;
    MovableOnly(MovableOnly&&) = default;
    MovableOnly& operator=(MovableOnly&&) = default;
};

// Performance test: Construction
void test_construction_performance() {
    std::cout << "Testing construction performance..." << std::endl;
    
    const int iterations = 1000000;
    
    // Test my_stl::pair construction
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            my_stl::pair<int, int> p(i, i * 2);
            (void)p; // Prevent optimization
        }
        double my_time = timer.elapsed();
        std::cout << "my_stl::pair construction: " << my_time << " ms" << std::endl;
    }
    
    // Test std::pair construction
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            std::pair<int, int> p(i, i * 2);
            (void)p; // Prevent optimization
        }
        double std_time = timer.elapsed();
        std::cout << "std::pair construction: " << std_time << " ms" << std::endl;
    }
    
    std::cout << "✓ Construction performance test completed" << std::endl;
}

// Performance test: Copy operations
void test_copy_performance() {
    std::cout << "Testing copy performance..." << std::endl;
    
    const int iterations = 1000000;
    my_stl::pair<int, int> my_source(42, 84);
    std::pair<int, int> std_source(42, 84);
    
    // Test my_stl::pair copy
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            my_stl::pair<int, int> p = my_source;
            (void)p; // Prevent optimization
        }
        double my_time = timer.elapsed();
        std::cout << "my_stl::pair copy: " << my_time << " ms" << std::endl;
    }
    
    // Test std::pair copy
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            std::pair<int, int> p = std_source;
            (void)p; // Prevent optimization
        }
        double std_time = timer.elapsed();
        std::cout << "std::pair copy: " << std_time << " ms" << std::endl;
    }
    
    std::cout << "✓ Copy performance test completed" << std::endl;
}

// Performance test: Move operations
void test_move_performance() {
    std::cout << "Testing move performance..." << std::endl;
    
    const int iterations = 100000;
    
    // Test my_stl::pair move
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            my_stl::pair<LargeObject, LargeObject> p1(LargeObject(i), LargeObject(i * 2));
            my_stl::pair<LargeObject, LargeObject> p2 = std::move(p1);
            (void)p2; // Prevent optimization
        }
        double my_time = timer.elapsed();
        std::cout << "my_stl::pair move: " << my_time << " ms" << std::endl;
    }
    
    // Test std::pair move
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            std::pair<LargeObject, LargeObject> p1(LargeObject(i), LargeObject(i * 2));
            std::pair<LargeObject, LargeObject> p2 = std::move(p1);
            (void)p2; // Prevent optimization
        }
        double std_time = timer.elapsed();
        std::cout << "std::pair move: " << std_time << " ms" << std::endl;
    }
    
    std::cout << "✓ Move performance test completed" << std::endl;
}

// Performance test: Assignment operations
void test_assignment_performance() {
    std::cout << "Testing assignment performance..." << std::endl;
    
    const int iterations = 1000000;
    
    // Test my_stl::pair assignment
    {
        my_stl::pair<int, int> p1(1, 2);
        my_stl::pair<int, int> p2(3, 4);
        
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            p1 = p2;
            p2.first = i;
        }
        double my_time = timer.elapsed();
        std::cout << "my_stl::pair assignment: " << my_time << " ms" << std::endl;
    }
    
    // Test std::pair assignment
    {
        std::pair<int, int> p1(1, 2);
        std::pair<int, int> p2(3, 4);
        
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            p1 = p2;
            p2.first = i;
        }
        double std_time = timer.elapsed();
        std::cout << "std::pair assignment: " << std_time << " ms" << std::endl;
    }
    
    std::cout << "✓ Assignment performance test completed" << std::endl;
}

// Performance test: make_pair
void test_make_pair_performance() {
    std::cout << "Testing make_pair performance..." << std::endl;
    
    const int iterations = 1000000;
    
    // Test my_stl::make_pair
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            auto p = my_stl::make_pair(i, i * 2);
            (void)p; // Prevent optimization
        }
        double my_time = timer.elapsed();
        std::cout << "my_stl::make_pair: " << my_time << " ms" << std::endl;
    }
    
    // Test std::make_pair
    {
        Timer timer;
        for (int i = 0; i < iterations; ++i) {
            auto p = std::make_pair(i, i * 2);
            (void)p; // Prevent optimization
        }
        double std_time = timer.elapsed();
        std::cout << "std::make_pair: " << std_time << " ms" << std::endl;
    }
    
    std::cout << "✓ make_pair performance test completed" << std::endl;
}

// Performance test: Container operations
void test_container_performance() {
    std::cout << "Testing container performance..." << std::endl;
    
    const int iterations = 100000;
    
    // Test my_stl::pair in vector
    {
        Timer timer;
        std::vector<my_stl::pair<int, int>> vec;
        vec.reserve(iterations);
        
        for (int i = 0; i < iterations; ++i) {
            vec.emplace_back(i, i * 2);
        }
        double my_time = timer.elapsed();
        std::cout << "my_stl::pair vector operations: " << my_time << " ms" << std::endl;
    }
    
    // Test std::pair in vector
    {
        Timer timer;
        std::vector<std::pair<int, int>> vec;
        vec.reserve(iterations);
        
        for (int i = 0; i < iterations; ++i) {
            vec.emplace_back(i, i * 2);
        }
        double std_time = timer.elapsed();
        std::cout << "std::pair vector operations: " << std_time << " ms" << std::endl;
    }
    
    std::cout << "✓ Container performance test completed" << std::endl;
}

// Performance test: Memory usage
void test_memory_usage() {
    std::cout << "Testing memory usage..." << std::endl;
    
    std::cout << "Size comparisons:" << std::endl;
    std::cout << "sizeof(my_stl::pair<int, int>): " << sizeof(my_stl::pair<int, int>) << " bytes" << std::endl;
    std::cout << "sizeof(std::pair<int, int>): " << sizeof(std::pair<int, int>) << " bytes" << std::endl;
    
    std::cout << "sizeof(my_stl::pair<char, char>): " << sizeof(my_stl::pair<char, char>) << " bytes" << std::endl;
    std::cout << "sizeof(std::pair<char, char>): " << sizeof(std::pair<char, char>) << " bytes" << std::endl;
    
    std::cout << "sizeof(my_stl::pair<double, double>): " << sizeof(my_stl::pair<double, double>) << " bytes" << std::endl;
    std::cout << "sizeof(std::pair<double, double>): " << sizeof(std::pair<double, double>) << " bytes" << std::endl;
    
    // Test EBCO effectiveness
    struct Empty {};
    std::cout << "sizeof(my_stl::pair<Empty, int>): " << sizeof(my_stl::pair<Empty, int>) << " bytes" << std::endl;
    std::cout << "sizeof(std::pair<Empty, int>): " << sizeof(std::pair<Empty, int>) << " bytes" << std::endl;
    
    std::cout << "✓ Memory usage test completed" << std::endl;
}

int main() {
    setup_console_encoding();
    std::cout << "=== my_stl::pair Performance Tests ===" << std::endl;
    
    try {
        test_construction_performance();
        std::cout << std::endl;
        
        test_copy_performance();
        std::cout << std::endl;
        
        test_move_performance();
        std::cout << std::endl;
        
        test_assignment_performance();
        std::cout << std::endl;
        
        test_make_pair_performance();
        std::cout << std::endl;
        
        test_container_performance();
        std::cout << std::endl;
        
        test_memory_usage();
        std::cout << std::endl;
        
        std::cout << "✅ All performance tests completed!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}