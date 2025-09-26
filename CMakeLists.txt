cmake_minimum_required(VERSION 3.15)
project(MySTL VERSION 1.0.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建头文件库
add_library(my_stl INTERFACE)

# 包含目录
target_include_directories(my_stl INTERFACE include)

# 单元测试（不依赖外部库）
add_executable(test_pair_basic test/unit/test_pair_basic.cpp)
target_link_libraries(test_pair_basic my_stl)

add_executable(test_pair_compatibility test/unit/test_pair_compatibility.cpp)
target_link_libraries(test_pair_compatibility my_stl)

add_executable(test_pair_ebco test/unit/test_pair_ebco.cpp)
target_link_libraries(test_pair_ebco my_stl)

add_executable(test_pair_performance test/unit/test_pair_performance.cpp)
target_link_libraries(test_pair_performance my_stl)

# 集成测试
add_executable(test_with_std test/integration/test_with_std.cpp)
target_link_libraries(test_with_std my_stl)

# 可选：如果有 benchmark 库，则编译性能测试
find_package(benchmark QUIET)
if(benchmark_FOUND)
    add_executable(benchmark_pair test/benchmark/benchmark_pair.cpp)
    target_link_libraries(benchmark_pair benchmark::benchmark my_stl)
    message(STATUS "Benchmark library found, building benchmark_pair")
else()
    message(STATUS "Benchmark library not found, skipping benchmark_pair")
endif()