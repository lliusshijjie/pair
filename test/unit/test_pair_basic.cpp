#include <iostream>
#include <cassert>
#include <string>
#include <utility>
#include <type_traits>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include "../../include/my_stl/utility.hpp"

// 设置控制台编码为UTF-8
void setup_console_encoding() {
#ifdef _WIN32
    // 设置控制台代码页为UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    // 设置标准输出为UTF-8模式
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);
#endif
}

// Simple test helper functions
void test_basic_construction() {
    std::cout << "=== Testing Basic Construction ===" << std::endl;
    
    // Default construction
    my_stl::pair<int, double> p1;
    assert(p1.first == 0);
    assert(p1.second == 0.0);
    std::cout << "Default construction test passed" << std::endl;
    
    // Value construction
    my_stl::pair<int, std::string> p2(42, "hello");
    assert(p2.first == 42);
    assert(p2.second == "hello");
    std::cout << "Value construction test passed" << std::endl;
    
    // Copy construction
    my_stl::pair<int, std::string> p3(p2);
    assert(p3.first == 42);
    assert(p3.second == "hello");
    std::cout << "Copy construction test passed" << std::endl;
    
    // Move construction
    my_stl::pair<int, std::string> p4(std::move(p3));
    assert(p4.first == 42);
    assert(p4.second == "hello");
    std::cout << "Move construction test passed" << std::endl;
}

void test_assignment() {
    std::cout << "\n=== Testing Assignment Operations ===" << std::endl;
    
    // Copy assignment
    my_stl::pair<int, std::string> p1(42, "hello");
    my_stl::pair<int, std::string> p2;
    p2 = p1;
    assert(p2.first == 42);
    assert(p2.second == "hello");
    std::cout << "Copy assignment test passed" << std::endl;
    
    // Move assignment
    my_stl::pair<int, std::string> p3(24, "world");
    my_stl::pair<int, std::string> p4;
    p4 = std::move(p3);
    assert(p4.first == 24);
    assert(p4.second == "world");
    std::cout << "Move assignment test passed" << std::endl;
}

void test_comparison_operators() {
    std::cout << "\n=== 测试比较操作符 ===" << std::endl;
    
    my_stl::pair<int, int> p1(1, 2);
    my_stl::pair<int, int> p2(1, 2);
    my_stl::pair<int, int> p3(1, 3);
    my_stl::pair<int, int> p4(2, 1);
    
    // 相等性
    assert(p1 == p2);
    assert(!(p1 == p3));
    assert(!(p1 == p4));
    std::cout << "相等性测试通过" << std::endl;
    
    // 不等性
    assert(!(p1 != p2));
    assert(p1 != p3);
    assert(p1 != p4);
    std::cout << "不等性测试通过" << std::endl;
    
    // 小于
    assert(p1 < p3);
    assert(p1 < p4);
    assert(!(p3 < p1));
    assert(!(p4 < p1));
    assert(!(p1 < p2));
    std::cout << "小于测试通过" << std::endl;
    
    // 小于等于
    assert(p1 <= p2);
    assert(p1 <= p3);
    assert(p1 <= p4);
    assert(!(p3 <= p1));
    assert(!(p4 <= p1));
    std::cout << "小于等于测试通过" << std::endl;
    
    // 大于
    assert(p3 > p1);
    assert(p4 > p1);
    assert(!(p1 > p3));
    assert(!(p1 > p4));
    assert(!(p1 > p2));
    std::cout << "大于测试通过" << std::endl;
    
    // 大于等于
    assert(p1 >= p2);
    assert(p3 >= p1);
    assert(p4 >= p1);
    assert(!(p1 >= p3));
    assert(!(p1 >= p4));
    std::cout << "大于等于测试通过" << std::endl;
}

void test_swap() {
    std::cout << "\n=== 测试swap操作 ===" << std::endl;
    
    my_stl::pair<int, std::string> p1(42, "hello");
    my_stl::pair<int, std::string> p2(24, "world");
    
    my_stl::swap(p1, p2);
    
    assert(p1.first == 24);
    assert(p1.second == "world");
    assert(p2.first == 42);
    assert(p2.second == "hello");
    std::cout << "swap测试通过" << std::endl;
}

void test_make_pair() {
    std::cout << "\n=== 测试make_pair函数 ===" << std::endl;
    
    auto p = my_stl::make_pair(42, 3.14);
    assert(p.first == 42);
    assert(p.second == 3.14);
    
    // 测试类型推导
    static_assert(std::is_same_v<decltype(p), my_stl::pair<int, double>>);
    std::cout << "make_pair测试通过" << std::endl;
}

void test_structured_binding() {
    std::cout << "\n=== 测试结构化绑定 ===" << std::endl;
    
    my_stl::pair<int, std::string> p(42, "hello");
    auto [first, second] = p;
    
    assert(first == 42);
    assert(second == "hello");
    std::cout << "结构化绑定测试通过" << std::endl;
}

void test_std_pair_compatibility() {
    std::cout << "\n=== 测试与std::pair的兼容性 ===" << std::endl;
    
    // 从std::pair构造
    std::pair<int, std::string> std_p(42, "hello");
    my_stl::pair<int, std::string> my_p(std_p);
    assert(my_p.first == 42);
    assert(my_p.second == "hello");
    std::cout << "从std::pair构造测试通过" << std::endl;
    
    // 转换为std::pair
    my_stl::pair<int, std::string> my_p2(24, "world");
    std::pair<int, std::string> std_p2 = my_p2;
    assert(std_p2.first == 24);
    assert(std_p2.second == "world");
    std::cout << "转换为std::pair测试通过" << std::endl;
    
    // 从std::pair赋值
    std::pair<int, std::string> std_p3(100, "test");
    my_stl::pair<int, std::string> my_p3;
    my_p3 = std_p3;
    assert(my_p3.first == 100);
    assert(my_p3.second == "test");
    std::cout << "从std::pair赋值测试通过" << std::endl;
}

void test_type_traits() {
    std::cout << "\n=== 测试类型特征 ===" << std::endl;
    
    using IntPair = my_stl::pair<int, int>;
    using StringPair = my_stl::pair<std::string, std::string>;
    
    // 测试基本类型特征
    static_assert(std::is_default_constructible_v<IntPair>);
    static_assert(std::is_copy_constructible_v<IntPair>);
    static_assert(std::is_move_constructible_v<IntPair>);
    static_assert(std::is_copy_assignable_v<IntPair>);
    static_assert(std::is_move_assignable_v<IntPair>);
    
    // 测试tuple特征
    static_assert(std::tuple_size_v<IntPair> == 2);
    static_assert(std::is_same_v<std::tuple_element_t<0, IntPair>, int>);
    static_assert(std::is_same_v<std::tuple_element_t<1, IntPair>, int>);
    
    std::cout << "类型特征测试通过" << std::endl;
}

void test_tuple_interface() {
    std::cout << "\n=== 测试tuple接口 ===" << std::endl;
    
    my_stl::pair<int, double> p{10, 3.14};
    
    // 使用std::get
    assert(std::get<0>(p) == 10);
    assert(std::get<1>(p) == 3.14);
    
    // 修改值
    std::get<0>(p) = 20;
    std::get<1>(p) = 2.71;
    
    assert(p.first == 20);
    assert(p.second == 2.71);
    
    // 测试const版本
    const my_stl::pair<int, double> cp{30, 1.41};
    assert(std::get<0>(cp) == 30);
    assert(std::get<1>(cp) == 1.41);
    
    std::cout << "tuple接口测试通过" << std::endl;
}

int main() {
    setup_console_encoding();
    std::cout << "Starting pair basic functionality unit tests..." << std::endl;
    
    try {
        test_basic_construction();
        test_assignment();
        test_comparison_operators();
        test_swap();
        test_make_pair();
        test_structured_binding();
        test_std_pair_compatibility();
        test_type_traits();
        test_tuple_interface();
        
        std::cout << "\nAll basic functionality unit tests passed! ✓" << std::endl;
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