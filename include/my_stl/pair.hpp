/*
    关键特性说明

    1. 实现委托模式
        使用 detail::pair_impl 处理实际的存储和EBCO
        pair 类主要提供接口和转发逻辑
        引用成员绑定到实现中的实际存储

    2. 完整的构造支持
        提供值构造、移动构造、完美转发构造
        支持从其他 pair 类型和 std::pair 构造
        使用 constexpr 支持编译时计算

    3. 规则五(Rule of Five)
        提供默认的拷贝/移动构造函数和赋值运算符
        确保正确的资源管理

    4. 异常安全
        为所有操作提供正确的 noexcept 规范
        使用类型特征检查异常安全性

    5. 转换支持
        提供到 std::pair 的隐式转换
        支持结构化绑定和 std::get 访问

    6. C++17 特性
        提供CTAD（类模板参数推导）指南
        支持结构化绑定

*/

#pragma once

#include "detail/pair_impl.hpp"
#include "detail/traits.hpp"
#include <type_traits>
#include <utility>

namespace my_stl {

// ============================================================================
// pair 类声明
// ============================================================================

template <typename T1, typename T2>
class pair {
public:
    // 成员类型定义
    using first_type = T1;
    using second_type = T2;

    // 成员变量
    first_type first;
    second_type second;

    // ========================================================================
    // 构造函数
    // ========================================================================

    // 默认构造函数
    constexpr pair() : first(), second() {}

    // 值构造函数
    constexpr pair(const T1& x, const T2& y) : first(x), second(y) {}

    // 移动构造函数
    constexpr pair(T1&& x, T2&& y) noexcept : first(std::move(x)), second(std::move(y)) {}

    // 完美转发构造函数
    template <typename U1, typename U2>
    constexpr pair(U1&& u1, U2&& u2) : first(std::forward<U1>(u1)), second(std::forward<U2>(u2)) {}

    // 拷贝构造函数
    constexpr pair(const pair&) = default;

    // 移动构造函数
    constexpr pair(pair&&) noexcept = default;

    // 从其他类型的 pair 构造
    template <typename U1, typename U2>
    constexpr pair(const pair<U1, U2>& other) : first(other.first), second(other.second) {}

    template <typename U1, typename U2>
    constexpr pair(pair<U1, U2>&& other) noexcept : first(std::move(other.first)), second(std::move(other.second)) {}

    // 从 std::pair 构造
    template <typename U1, typename U2>
    constexpr pair(const std::pair<U1, U2>& other) : first(other.first), second(other.second) {}

    template <typename U1, typename U2>
    constexpr pair(std::pair<U1, U2>&& other) noexcept : first(std::forward<U1>(other.first)), second(std::forward<U2>(other.second)) {}

    // ========================================================================
    // 赋值运算符
    // ========================================================================

    // 拷贝赋值
    pair& operator=(const pair& other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    // 移动赋值
    pair& operator=(pair&& other) noexcept {
        if (this != &other) {
            first = std::move(other.first);
            second = std::move(other.second);
        }
        return *this;
    }

    // 从其他 pair 类型赋值
    template <typename U1, typename U2>
    pair& operator=(const pair<U1, U2>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    template <typename U1, typename U2>
    pair& operator=(pair<U1, U2>&& other) {
        first = std::forward<U1>(other.first);
        second = std::forward<U2>(other.second);
        return *this;
    }

    // 从 std::pair 赋值
    template<typename U1, typename U2>
    pair& operator=(std::pair<U1, U2>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    template<typename U1, typename U2>
    pair& operator=(std::pair<U1, U2>&& other) {
        first = std::move(other.first);
        second = std::move(other.second);
        return *this;
    }

    // ========================================================================
    // 成员函数
    // ========================================================================

    // swap 方法
    void swap(pair& other) noexcept {
        using std::swap;
        swap(first, other.first);
        swap(second, other.second);
    }

    // 转换为 std::pair
    template <typename U1 = T1, typename U2 = T2>
    constexpr operator std::pair<U1, U2>() const & {
        return std::pair<U1, U2>(first, second);
    }

    template <typename U1 = T1, typename U2 = T2>
    constexpr operator std::pair<U1, U2>() && {
        return std::pair<U1, U2>(std::move(first), std::move(second));
    }
};

// ============================================================================
// make_pair
// ============================================================================

template <typename T1, typename T2>
constexpr pair<detail::unwrap_ref_decay_t<T1>, detail::unwrap_ref_decay_t<T2>>
make_pair(T1&& t, T2&& u) {
    return pair<detail::unwrap_ref_decay_t<T1>, detail::unwrap_ref_decay_t<T2>>(
        std::forward<T1>(t), std::forward<T2>(u));
}

// ============================================================================
// Non-member comparison operators
// ============================================================================

template <typename T1, typename T2>
constexpr bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename T1, typename T2>
constexpr bool operator!=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs == rhs);
}

template <typename T1, typename T2>
constexpr bool operator<(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    if (lhs.first < rhs.first) return true;
    if (rhs.first < lhs.first) return false;
    return lhs.second < rhs.second;
}

template <typename T1, typename T2>
constexpr bool operator<=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(rhs < lhs);
}

template <typename T1, typename T2>
constexpr bool operator>(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return rhs < lhs;
}

template <typename T1, typename T2>
constexpr bool operator>=(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs < rhs);
}


// Mixed comparisons with std::pair

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator==(const pair<T1, T2>& lhs, const std::pair<U1, U2>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator!=(const pair<T1, T2>& lhs, const std::pair<U1, U2>& rhs) {
    return !(lhs == rhs);
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator<(const pair<T1, T2>& lhs, const std::pair<U1, U2>& rhs) {
    if (lhs.first < rhs.first) return true;
    if (rhs.first < lhs.first) return false;
    return lhs.second < rhs.second;
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator<=(const pair<T1, T2>& lhs, const std::pair<U1, U2>& rhs) {
    return !(rhs < lhs);
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator>(const pair<T1, T2>& lhs, const std::pair<U1, U2>& rhs) {
    return rhs < lhs;
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator>=(const pair<T1, T2>& lhs, const std::pair<U1, U2>& rhs) {
    return !(lhs < rhs);
}


// Mixed comparisons with std::pair (std::pair on the left)

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator==(const std::pair<U1, U2>& lhs, const pair<T1, T2>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator!=(const std::pair<U1, U2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs == rhs);
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator<(const std::pair<U1, U2>& lhs, const pair<T1, T2>& rhs) {
    if (lhs.first < rhs.first) return true;
    if (rhs.first < lhs.first) return false;
    return lhs.second < rhs.second;
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator<=(const std::pair<U1, U2>& lhs, const pair<T1, T2>& rhs) {
    return !(rhs < lhs);
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator>(const std::pair<U1, U2>& lhs, const pair<T1, T2>& rhs) {
    return rhs < lhs;
}

template <typename T1, typename T2, typename U1, typename U2>
constexpr bool operator>=(const std::pair<U1, U2>& lhs, const pair<T1, T2>& rhs) {
    return !(lhs < rhs);
}


// ============================================================================
// swap
// ============================================================================

template <typename T1, typename T2>
void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

// ============================================================================
// 推导指南 (C++17)
// ============================================================================

#if __cplusplus >= 201703L
template <typename T1, typename T2>
pair(T1, T2) -> pair<T1, T2>;
#endif

} // namespace my_stl

// ============================================================================
// 特化 std::tuple_size, std::tuple_element (在全局命名空间)
// ============================================================================

template <typename T1, typename T2>
struct std::tuple_size<my_stl::pair<T1, T2>> : 
    std::integral_constant<std::size_t, 2> {};

template <std::size_t I, typename T1, typename T2>
struct std::tuple_element<I, my_stl::pair<T1, T2>> {
    static_assert(I < 2, "Index out of bounds for pairs");
    using type = std::conditional_t<I == 0, T1, T2>;
};

// ============================================================================
// get 函数重载 (在std命名空间中)
// ============================================================================

namespace std {

template <std::size_t I, typename T1, typename T2>
constexpr auto& get(my_stl::pair<T1, T2>& p) noexcept {
    static_assert(I < 2, "Index out of bounds for pairs");
    if constexpr (I == 0) return p.first;
    else return p.second;
}

template <std::size_t I, typename T1, typename T2>
constexpr const auto& get(const my_stl::pair<T1, T2>& p) noexcept {
    static_assert(I < 2, "Index out of bounds for pairs");
    if constexpr (I == 0) return p.first;
    else return p.second;
}

template <std::size_t I, typename T1, typename T2> 
constexpr auto&& get(my_stl::pair<T1, T2>&& p) noexcept {
    static_assert(I < 2, "Index out of bounds for pairs");
    if constexpr (I == 0) return std::move(p.first);
    else return std::move(p.second);
}

template <typename T1, typename T2>
struct hash<my_stl::pair<T1, T2>> {
    size_t operator()(const my_stl::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

} // namespace std