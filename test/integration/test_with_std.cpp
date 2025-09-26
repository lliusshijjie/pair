#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <functional>
#include <tuple>
#include <type_traits>
#include <cassert>
#include <string>
#include <utility>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include "../../include/my_stl/utility.hpp"

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

// Test type that may throw exceptions
struct ThrowingType {
    int value;
    static bool should_throw;
    
    ThrowingType(int v = 0) : value(v) {
        if (should_throw) {
            throw std::runtime_error("Construction failed");
        }
    }
    
    ThrowingType(const ThrowingType& other) : value(other.value) {
        if (should_throw) {
            throw std::runtime_error("Copy failed");
        }
    }
    
    ThrowingType& operator=(const ThrowingType& other) {
        if (should_throw) {
            throw std::runtime_error("Assignment failed");
        }
        value = other.value;
        return *this;
    }
};

bool ThrowingType::should_throw = false;

// Test integration with STL containers
void test_stl_containers() {
    std::cout << "=== Testing STL Container Integration ===" << std::endl;
    
    // Test vector
    std::vector<my_stl::pair<int, std::string>> vec;
    vec.emplace_back(1, "one");
    vec.emplace_back(2, "two");
    vec.emplace_back(3, "three");
    
    assert(vec.size() == 3);
    assert(vec[0].first == 1 && vec[0].second == "one");
    
    // Test sorting
    std::sort(vec.begin(), vec.end(), [](const auto& a, const auto& b) {
        return a.first > b.first; // descending order
    });
    assert(vec[0].first == 3);
    
    // Test finding
    auto it = std::find_if(vec.begin(), vec.end(), [](const auto& p) {
        return p.second == "two";
    });
    assert(it != vec.end() && it->first == 2);
    
    std::cout << "STL container integration test passed!" << std::endl;
}

// Test integration with map
void test_map_integration() {
    std::cout << "\n=== Testing Map Integration ===" << std::endl;
    
    // Use my_stl::pair as map's value_type
    std::map<int, my_stl::pair<std::string, double>> score_map;
    
    score_map[1] = my_stl::make_pair(std::string("Alice"), 95.5);
    score_map[2] = my_stl::make_pair(std::string("Bob"), 87.3);
    score_map[3] = my_stl::make_pair(std::string("Charlie"), 92.1);
    
    assert(score_map.size() == 3);
    assert(score_map[1].first == "Alice");
    assert(score_map[1].second == 95.5);
    
    // Test iteration
    for (const auto& entry : score_map) {
        assert(entry.second.first.length() > 0);
        assert(entry.second.second > 0);
    }
    
    std::cout << "Map integration test passed!" << std::endl;
}

// Test integration with unordered_map
void test_unordered_map_integration() {
    std::cout << "\n=== Testing Unordered Map Integration ===" << std::endl;
    
    // Use my_stl::pair as key (requires custom hash)
    struct PairHash {
        std::size_t operator()(const my_stl::pair<int, int>& p) const {
            return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
        }
    };
    
    std::unordered_map<my_stl::pair<int, int>, std::string, PairHash> coord_map;
    
    coord_map[my_stl::make_pair(0, 0)] = "origin";
    coord_map[my_stl::make_pair(1, 1)] = "diagonal";
    coord_map[my_stl::make_pair(-1, 0)] = "left";
    
    assert(coord_map.size() == 3);
    assert(coord_map[my_stl::make_pair(0, 0)] == "origin");
    
    // Test finding
    auto it = coord_map.find(my_stl::make_pair(1, 1));
    assert(it != coord_map.end() && it->second == "diagonal");
    
    std::cout << "Unordered map integration test passed!" << std::endl;
}

// Test integration with set
void test_set_integration() {
    std::cout << "\n=== Testing Set Integration ===" << std::endl;
    
    std::set<my_stl::pair<int, std::string>> pair_set;
    
    pair_set.insert(my_stl::make_pair(3, std::string("three")));
    pair_set.insert(my_stl::make_pair(1, std::string("one")));
    pair_set.insert(my_stl::make_pair(2, std::string("two")));
    pair_set.insert(my_stl::make_pair(1, std::string("one"))); // duplicate insertion
    
    assert(pair_set.size() == 3); // duplicate elements won't be inserted
    
    // Verify sorting (set automatically sorts)
    auto it = pair_set.begin();
    assert(it->first == 1);
    ++it;
    assert(it->first == 2);
    ++it;
    assert(it->first == 3);
    
    std::cout << "Set integration test passed!" << std::endl;
}

// Test type traits
void test_type_traits() {
    std::cout << "\n=== Testing Type Traits ===" << std::endl;
    
    using IntPair = my_stl::pair<int, int>;
    using StringPair = my_stl::pair<std::string, std::string>;
    
    // Test basic type traits
    static_assert(std::is_default_constructible_v<IntPair>);
    static_assert(std::is_copy_constructible_v<IntPair>);
    static_assert(std::is_move_constructible_v<IntPair>);
    static_assert(std::is_copy_assignable_v<IntPair>);
    static_assert(std::is_move_assignable_v<IntPair>);
    
    // Test tuple traits
    static_assert(std::tuple_size_v<IntPair> == 2);
    static_assert(std::is_same_v<std::tuple_element_t<0, IntPair>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<1, IntPair>, int>);
    
    std::cout << "Type traits test passed!" << std::endl;
}

// Test structured binding
void test_structured_binding() {
    std::cout << "\n=== Testing Structured Binding ===" << std::endl;
    
    my_stl::pair<int, std::string> p{42, "answer"};
    
    // C++17 structured binding
    auto [num, text] = p;
    assert(num == 42);
    assert(text == "answer");
    
    // Modify original pair
    p.first = 100;
    p.second = "modified";
    
    // Bound variables are not affected (copy semantics)
    assert(num == 42);
    assert(text == "answer");
    
    // Reference binding
    auto& [ref_num, ref_text] = p;
    assert(ref_num == 100);
    assert(ref_text == "modified");
    
    // Modifying references affects the original pair
    ref_num = 200;
    assert(p.first == 200);
    
    std::cout << "Structured binding test passed!" << std::endl;
}

// Test interoperability with std::tuple
void test_tuple_interop() {
    std::cout << "\n=== Testing Tuple Interoperability ===" << std::endl;
    
    my_stl::pair<int, double> p{10, 3.14};
    
    // Use std::get
    assert(std::get<0>(p) == 10);
    assert(std::get<1>(p) == 3.14);
    
    // Modify values
    std::get<0>(p) = 20;
    std::get<1>(p) = 2.71;
    
    assert(p.first == 20);
    assert(p.second == 2.71);
    
    // Test const version
    const my_stl::pair<int, double> cp{30, 1.41};
    assert(std::get<0>(cp) == 30);
    assert(std::get<1>(cp) == 1.41);
    
    std::cout << "Tuple interoperability test passed!" << std::endl;
}

// Test exception safety
void test_exception_safety() {
    std::cout << "\n=== Testing Exception Safety ===" << std::endl;
    
    ThrowingType::should_throw = false;
    
    // Normal case
    my_stl::pair<ThrowingType, int> p1{ThrowingType{42}, 100};
    assert(p1.first.value == 42);
    assert(p1.second == 100);
    
    // Test exception safety of copy construction
    ThrowingType::should_throw = true;
    try {
        my_stl::pair<ThrowingType, int> p2{p1}; // should throw exception
        assert(false); // should not reach here
    }
    catch (const std::runtime_error&) {
        // expected exception
    }
    
    ThrowingType::should_throw = false;
    std::cout << "Exception safety test passed!" << std::endl;
}

int main() {
    setup_console_encoding();
    std::cout << "Starting standard library integration tests..." << std::endl;
    
    try {
        test_stl_containers();
        test_map_integration();
        test_unordered_map_integration();
        test_set_integration();
        test_type_traits();
        test_structured_binding();
        test_tuple_interop();
        test_exception_safety();
        
        std::cout << "\nAll standard library integration tests passed! ✓" << std::endl;
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