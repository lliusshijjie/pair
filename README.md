# MySTL Pair Implementation

一个现代化的C++ STL风格pair实现，提供完整的pair容器功能，支持C++17/20特性，并包含空基类优化(EBCO)等性能优化。

## 项目特性

### 🚀 核心特性
- **完整的STL兼容性**: 与标准库`std::pair`完全兼容
- **异常安全**: 所有操作都提供正确的异常安全保证
- **规则五(Rule of Five)**: 完整的拷贝/移动语义支持
- **委托模式设计**: 使用`detail::pair_impl`处理实际存储逻辑

### 🔧 现代C++支持
- **C++17结构化绑定**: 支持`auto [first, second] = pair`语法
- **完美转发**: 保持值类别的构造函数和赋值操作
- **constexpr支持**: 编译时计算支持
- **tuple接口兼容**: 支持`std::get`和tuple相关操作

### 📊 兼容性特性
- **与std::pair互操作**: 支持从`std::pair`构造和转换
- **类型特征支持**: 完整的类型特征检查
- **比较操作符**: 完整的字典序比较支持

## 项目结构

```
pair/
├── include/
│   └── my_stl/
│       ├── pair.hpp          # 主要的pair类接口
│       ├── utility.hpp       # 工具函数(make_pair, swap等)
│       └── detail/           # 实现细节目录
├── test/
│   ├── unit/                 # 单元测试
│   │   ├── test_pair_basic.cpp
│   │   ├── test_pair_compatibility.cpp
│   │   ├── test_pair_ebco.cpp
│   │   └── test_pair_performance.cpp
│   ├── integration/          # 集成测试
│   │   └── test_with_std.cpp
│   └── benchmark/            # 性能测试
│       └── benchmark_pair.cpp
├── CMakeLists.txt           # 构建配置
└── README.md               # 项目文档
```

## 快速开始

### 基本使用

```cpp
#include "my_stl/utility.hpp"
#include <iostream>

int main() {
    // 值构造
    my_stl::pair<int, std::string> p1(42, "hello");
    
    // CTAD支持 (C++17)
    my_stl::pair p2(3.14, "world");
    
    // make_pair函数
    auto p3 = my_stl::make_pair(1, 2.0);
    
    // 结构化绑定 (C++17)
    auto [first, second] = p1;
    std::cout << first << ", " << second << std::endl;
    
    return 0;
}
```

### 比较操作

```cpp
my_stl::pair<int, int> a(1, 2);
my_stl::pair<int, int> b(1, 3);

// 字典序比较
if (a < b) {
    std::cout << "a < b" << std::endl;
}

// 与std::pair互操作
std::pair<int, int> c(1, 2);
if (a == c) {
    std::cout << "a == c" << std::endl;
}
```

### 与std::pair互操作示例

```cpp
// 从std::pair构造
std::pair<int, std::string> std_pair(42, "hello");
my_stl::pair<int, std::string> my_pair(std_pair);

// 转换为std::pair
my_stl::pair<int, std::string> my_pair2(24, "world");
std::pair<int, std::string> std_pair2 = my_pair2;

// 比较操作
if (my_pair == std_pair) {
    std::cout << "Pairs are equal" << std::endl;
}
```

## 构建和测试

### 构建项目

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 构建
cmake --build .
```

### 运行测试

```bash
# 启用测试
cmake -DBUILD_TESTING=ON ..

# 构建并运行测试
cmake --build .
ctest
```

### 运行特定测试

```bash
# 运行基本功能测试
./test/unit/test_pair_basic

# 运行兼容性测试
./test/unit/test_pair_compatibility

# 运行性能测试
./test/unit/test_pair_performance
```

## API参考

### 主要类

#### `my_stl::pair<T1, T2>`

**成员类型:**
- `first_type`: 第一个元素的类型
- `second_type`: 第二个元素的类型

**成员变量:**
- `first`: 第一个元素的引用
- `second`: 第二个元素的引用

**构造函数:**
- 默认构造、值构造、拷贝构造、移动构造
- 完美转发构造、从其他pair类型构造
- 从`std::pair`构造

**赋值操作:**
- 拷贝赋值、移动赋值
- 从其他pair类型赋值
- 从`std::pair`赋值

### 工具函数

#### `my_stl::make_pair(T1&& t1, T2&& t2)`
创建pair对象，自动推导类型。

#### `my_stl::swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs)`
交换两个pair对象的内容。

### 比较操作符

支持所有标准比较操作符：`==`, `!=`, `<`, `<=`, `>`, `>=`

### 结构化绑定支持

支持C++17结构化绑定，可以直接使用`auto [first, second] = pair`语法。

## 技术细节

### 实现架构

项目采用委托模式设计，pair类主要提供接口和转发逻辑，实际的存储和逻辑处理由底层实现负责。

### 异常安全保证

- **基本保证**: 所有操作在异常发生时保持对象有效性
- **强保证**: 赋值操作等提供强异常安全保证
- **无异常保证**: 移动操作等提供无异常保证

### 类型特征支持

项目包含完整的类型特征工具，支持：
- 异常安全检查
- 移动语义支持检查
- SFINAE友好接口
- tuple接口兼容性检查

## 兼容性

- **C++标准**: C++14及以上（C++17推荐）
- **编译器**: GCC 7+, Clang 5+, MSVC 2017+
- **标准库**: 与任何符合标准的STL实现兼容
- **测试框架**: 使用Catch2进行单元测试，Google Benchmark进行性能测试

## 测试覆盖

项目包含完整的测试套件：

- **基本功能测试**: 构造、赋值、比较等基本操作
- **兼容性测试**: 与std::pair的互操作性
- **性能测试**: 性能基准测试
- **集成测试**: 与标准库的集成测试

### 运行测试

所有测试都可以通过CMake配置和运行，确保代码质量和功能正确性。

## 许可证

本项目采用MIT许可证。详见LICENSE文件。

## 相关资源

- [C++标准文档](https://isocpp.org/)
- [STL参考](https://en.cppreference.com/w/cpp/utility/pair)
- [EBCO技术说明](https://en.wikipedia.org/wiki/Empty_base_optimization)

---

**注意**: 这是一个教育性质的STL实现，适用于学习和研究目的。生产环境建议使用标准库实现。