#include <iostream>
#include <cassert>
#include <utility>
#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <unordered_map>
#include <functional>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include "../../include/my_stl/pair.hpp"

// 设置控制台编码为UTF-8
void setup_console_encoding() {
#ifdef _WIN32
    // 设置控制台代码页为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // 设置标准输出为UTF-8模式
    // _setmode(_fileno(stdout), _O_U8TEXT);
    // _setmode(_fileno(stderr), _O_U8TEXT);
#endif
}

void test_std_pair_construction() {
    std::cout << "=== Testing construction from std::pair ===" << std::endl;
    
    // Copy construction from std::pair
    std::pair<int, std::string> std_p1(42, "hello");
    my_stl::pair<int, std::string> my_p1(std_p1);
    assert(my_p1.first == 42);
    assert(my_p1.second == "hello");
    std::cout << "Copy construction from std::pair test passed" << std::endl;
    
    // Move construction from std::pair
    std::pair<int, std::string> std_p2(99, "world");
    my_stl::pair<int, std::string> my_p2(std::move(std_p2));
    assert(my_p2.first == 99);
    assert(my_p2.second == "world");
    std::cout << "Move construction from std::pair test passed" << std::endl;
}

void test_std_pair_assignment() {
    std::cout << "\n=== Testing assignment from std::pair ===" << std::endl;
    
    // Copy assignment from std::pair
    std::pair<int, std::string> std_p1(42, "hello");
    my_stl::pair<int, std::string> my_p1;
    my_p1 = std_p1;
    assert(my_p1.first == 42);
    assert(my_p1.second == "hello");
    std::cout << "Copy assignment from std::pair test passed" << std::endl;
    
    // Move assignment from std::pair
    std::pair<int, std::string> std_p2(99, "world");
    my_stl::pair<int, std::string> my_p2;
    my_p2 = std::move(std_p2);
    assert(my_p2.first == 99);
    assert(my_p2.second == "world");
    std::cout << "Move assignment from std::pair test passed" << std::endl;
}

void test_conversion_to_std_pair() {
    std::cout << "\n=== Testing conversion to std::pair ===" << std::endl;
    
    // Implicit conversion to std::pair
    my_stl::pair<int, std::string> my_p(42, "hello");
    std::pair<int, std::string> std_p = my_p;
    assert(std_p.first == 42);
    assert(std_p.second == "hello");
    std::cout << "Implicit conversion to std::pair test passed" << std::endl;
    
    // Explicit conversion to std::pair
    my_stl::pair<int, std::string> my_p2(24, "world");
    std::pair<int, std::string> std_p2 = static_cast<std::pair<int, std::string>>(my_p2);
    assert(std_p2.first == 24);
    assert(std_p2.second == "world");
    std::cout << "Explicit conversion to std::pair test passed" << std::endl;
}

void test_mixed_comparisons() {
    std::cout << "\n=== Testing mixed comparisons ===" << std::endl;
    
    my_stl::pair<int, int> my_p(1, 2);
    std::pair<int, int> std_p(1, 2);
    std::pair<int, int> std_p2(1, 3);
    
    // Compare my_stl::pair with std::pair
    assert(my_p == std_p);
    assert(!(my_p == std_p2));
    assert(!(my_p != std_p));
    assert(my_p != std_p2);
    assert(my_p < std_p2);
    assert(!(std_p2 < my_p));
    std::cout << "Mixed comparison test passed" << std::endl;
}

void test_stl_container_compatibility() {
    std::cout << "\n=== Testing STL container compatibility ===" << std::endl;
    
    // Use in vector
    std::vector<my_stl::pair<int, std::string>> vec;
    vec.emplace_back(1, "one");
    vec.emplace_back(2, "two");
    assert(vec.size() == 2);
    assert(vec[0].first == 1 && vec[0].second == "one");
    std::cout << "Vector compatibility test passed" << std::endl;
    
    // Use as value in map
    std::map<int, my_stl::pair<std::string, double>> score_map;
    score_map[1] = my_stl::make_pair(std::string("Alice"), 95.5);
    score_map[2] = my_stl::make_pair(std::string("Bob"), 87.3);
    assert(score_map.size() == 2);
    assert(score_map[1].first == "Alice");
    assert(score_map[1].second == 95.5);
    std::cout << "Map compatibility test passed" << std::endl;
}

void test_hash_compatibility() {
    std::cout << "\n=== Testing hash compatibility ===" << std::endl;
    
    my_stl::pair<int, int> p1(1, 2);
    my_stl::pair<int, int> p2(1, 2);
    my_stl::pair<int, int> p3(2, 1);
    
    std::hash<my_stl::pair<int, int>> hasher;
    auto h1 = hasher(p1);
    auto h2 = hasher(p2);
    auto h3 = hasher(p3);
    
    // Same pairs should have same hash values
    assert(h1 == h2);
    // Different pairs usually have different hash values (not absolutely guaranteed, but should hold in this simple example)
    assert(h1 != h3);
    std::cout << "Hash compatibility test passed" << std::endl;
}

void test_type_deduction_compatibility() {
    std::cout << "\n=== Testing type deduction compatibility ===" << std::endl;
    
    // Test auto deduction
    auto my_p = my_stl::make_pair(42, 3.14);
    auto std_p = std::make_pair(42, 3.14);
    
    assert(my_p.first == std_p.first);
    assert(my_p.second == std_p.second);
    std::cout << "Type deduction compatibility test passed" << std::endl;
}

void test_perfect_forwarding() {
    std::cout << "\n=== Testing perfect forwarding ===" << std::endl;
    
    // Test perfect forwarding of rvalue references
    std::string s = "test";
    auto p1 = my_stl::make_pair(42, std::move(s));
    assert(p1.first == 42);
    assert(p1.second == "test");
    // s should be moved, but we can't directly test this
    
    // Test lvalue references
    int x = 100;
    std::string y = "hello";
    auto p2 = my_stl::make_pair(x, y);
    assert(p2.first == 100);
    assert(p2.second == "hello");
    // x and y should remain unchanged
    assert(x == 100);
    assert(y == "hello");
    
    std::cout << "Perfect forwarding test passed" << std::endl;
}

void test_const_correctness() {
    std::cout << "\n=== Testing const correctness ===" << std::endl;
    
    const my_stl::pair<int, std::string> cp(42, "hello");
    
    // const member access
    assert(cp.first == 42);
    assert(cp.second == "hello");
    
    // const version comparison
    my_stl::pair<int, std::string> p(42, "hello");
    assert(cp == p);
    assert(p == cp);
    
    // const version of std::get
    assert(std::get<0>(cp) == 42);
    assert(std::get<1>(cp) == "hello");
    
    std::cout << "Const correctness test passed" << std::endl;
}

int main() {
    setup_console_encoding();
    std::cout << "Starting pair compatibility unit tests..." << std::endl;
    
    try {
        test_std_pair_construction();
        test_std_pair_assignment();
        test_conversion_to_std_pair();
        test_mixed_comparisons();
        test_stl_container_compatibility();
        test_hash_compatibility();
        test_type_deduction_compatibility();
        test_perfect_forwarding();
        test_const_correctness();
        
        std::cout << "\nAll compatibility unit tests passed! ✓" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown error occurred" << std::endl;
        return 1;
    }
}