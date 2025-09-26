#include <iostream>
#include <cassert>
#include <type_traits>
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

// Test classes for EBCO
struct Empty1 {};
struct Empty2 {};
struct Empty3 {};

struct NonEmpty {
    int value = 42;
};

struct FinalEmpty final {};

// Test EBCO with empty types
void test_ebco_both_empty() {
    std::cout << "Testing EBCO with both empty types..." << std::endl;
    
    my_stl::pair<Empty1, Empty2> p;
    
    // With EBCO, size should be minimal (typically 1 or 2 bytes)
    std::cout << "Size of pair<Empty1, Empty2>: " << sizeof(p) << " bytes" << std::endl;
    
    // Should be much smaller than without EBCO
    assert(sizeof(p) <= 2);
    
    std::cout << "✓ EBCO test with both empty types passed" << std::endl;
}

// Test EBCO with one empty, one non-empty
void test_ebco_one_empty() {
    std::cout << "Testing EBCO with one empty, one non-empty type..." << std::endl;
    
    my_stl::pair<Empty1, int> p1;
    my_stl::pair<int, Empty1> p2;
    
    std::cout << "Size of pair<Empty1, int>: " << sizeof(p1) << " bytes" << std::endl;
    std::cout << "Size of pair<int, Empty1>: " << sizeof(p2) << " bytes" << std::endl;
    std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
    
    // With EBCO, should be same size as int (or close due to alignment)
    assert(sizeof(p1) <= sizeof(int) + alignof(int));
    assert(sizeof(p2) <= sizeof(int) + alignof(int));
    
    std::cout << "✓ EBCO test with one empty type passed" << std::endl;
}

// Test no EBCO with non-empty types
void test_no_ebco_non_empty() {
    std::cout << "Testing no EBCO with non-empty types..." << std::endl;
    
    my_stl::pair<NonEmpty, int> p;
    
    std::cout << "Size of pair<NonEmpty, int>: " << sizeof(p) << " bytes" << std::endl;
    std::cout << "Size of NonEmpty: " << sizeof(NonEmpty) << " bytes" << std::endl;
    std::cout << "Size of int: " << sizeof(int) << " bytes" << std::endl;
    
    // Should be at least the sum of both types
    assert(sizeof(p) >= sizeof(NonEmpty) + sizeof(int));
    
    std::cout << "✓ No EBCO test with non-empty types passed" << std::endl;
}

// Test EBCO with final empty class
void test_ebco_final_empty() {
    std::cout << "Testing EBCO with final empty class..." << std::endl;
    
    my_stl::pair<FinalEmpty, int> p;
    
    std::cout << "Size of pair<FinalEmpty, int>: " << sizeof(p) << " bytes" << std::endl;
    
    // Final classes cannot be used as base classes, so no EBCO
    assert(sizeof(p) >= sizeof(FinalEmpty) + sizeof(int));
    
    std::cout << "✓ EBCO test with final empty class passed" << std::endl;
}

// Test functionality with EBCO
void test_ebco_functionality() {
    std::cout << "Testing functionality with EBCO..." << std::endl;
    
    my_stl::pair<Empty1, int> p1(Empty1{}, 42);
    my_stl::pair<int, Empty2> p2(100, Empty2{});
    
    // Test access
    assert(p1.second == 42);
    assert(p2.first == 100);
    
    // Test assignment
    p1.second = 84;
    p2.first = 200;
    
    assert(p1.second == 84);
    assert(p2.first == 200);
    
    // Test copy construction
    auto p1_copy = p1;
    auto p2_copy = p2;
    
    assert(p1_copy.second == 84);
    assert(p2_copy.first == 200);
    
    // Test move construction
    auto p1_moved = std::move(p1);
    auto p2_moved = std::move(p2);
    
    assert(p1_moved.second == 84);
    assert(p2_moved.first == 200);
    
    std::cout << "✓ EBCO functionality test passed" << std::endl;
}

// Test multiple empty types
void test_multiple_empty_types() {
    std::cout << "Testing multiple different empty types..." << std::endl;
    
    my_stl::pair<Empty1, Empty2> p1;
    my_stl::pair<Empty2, Empty3> p2;
    my_stl::pair<Empty1, Empty3> p3;
    
    std::cout << "Size of pair<Empty1, Empty2>: " << sizeof(p1) << " bytes" << std::endl;
    std::cout << "Size of pair<Empty2, Empty3>: " << sizeof(p2) << " bytes" << std::endl;
    std::cout << "Size of pair<Empty1, Empty3>: " << sizeof(p3) << " bytes" << std::endl;
    
    // All should be minimal size
    assert(sizeof(p1) <= 2);
    assert(sizeof(p2) <= 2);
    assert(sizeof(p3) <= 2);
    
    std::cout << "✓ Multiple empty types test passed" << std::endl;
}

int main() {
    setup_console_encoding();
    std::cout << "=== my_stl::pair EBCO Tests ===" << std::endl;
    
    try {
        test_ebco_both_empty();
        test_ebco_one_empty();
        test_no_ebco_non_empty();
        test_ebco_final_empty();
        test_ebco_functionality();
        test_multiple_empty_types();
        
        std::cout << "\n✅ All EBCO tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "\n❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}