/*
    关键特性说明
    1. 空基类优化(EBCO)策略
        使用模板特化根据类型特征选择不同的实现
        对空且非final的类型启用EBCO
        避免对引用和数组类型错误应用EBCO

    2. 异常安全
        为所有操作提供正确的 noexcept 规范
        使用 std::is_nothrow_swappable_v 等类型特征

    3. 完美转发支持
        构造函数使用完美转发保持值类别
        支持各种构造场景

    4. 规则五(Rule of Five)
        提供默认的拷贝/移动构造函数和赋值运算符
        确保正确的资源管理

    5. 引用类型特化
        防止对引用类型错误应用EBCO
        引用类型总是使用非EBCO版本

    6. 数组类型保护
        静态断言防止直接使用数组类型
        引导用户使用指针或 std::array
*/

#pragma once

#include "traits.hpp"
#include <type_traits>
#include <utility>

namespace my_stl::detail {

// ============================================================================
// 基础实现模板 - 根据是否启用EBCO进行特化
// ============================================================================

template <typename T1, typename T2, bool FirstEmpty, bool SecondEmpty>
class pair_impl_base;

// ============================================================================
// 情况1: 两个类型都不是空类或都是final类 (不启用EBCO)
// ============================================================================
template <typename T1, typename T2>
class pair_impl_base<T1, T2, false, false> {
protected:
    T1 first_;
    T2 second_;

public:
    // 默认构造函数
    constexpr pair_impl_base() = default;

    // 值构造函数
    template <typename U1, typename U2>
    constexpr pair_impl_base(U1&& u1, U2&& u2) : first_(std::forward<U1>(u1)),
                                                 second_(std::forward<U2>(u2)) {}                                                                        
    
    // 拷贝构造函数
    constexpr pair_impl_base(const pair_impl_base&) = default;

    // 移动构造函数
    constexpr pair_impl_base(pair_impl_base&&) noexcept = default;

    // 析构函数
    ~pair_impl_base() = default;

    // 访问成员
    constexpr T1& first() noexcept { return first_; }
    constexpr T2& second() noexcept { return second_; }

    constexpr const T1& first() const noexcept { return first_; }
    constexpr const T2& second() const noexcept { return second_; }

    // 交换操作
    void swap(pair_impl_base& other) noexcept(is_nothrow_swappable_v<T1> && is_nothrow_swappable_v<T2>) {
        using std::swap;
        swap(first_, other.first_);
        swap(second_, other.second_);
    }
};

// ============================================================================
// 情况2: 只有第一个类型是空类且非final (启用EBCO)
// ============================================================================

template <typename T1, typename T2>
class pair_impl_base<T1, T2, true, false> : private T1 {
protected:
    T2 second_;

public:
    // 默认构造函数
    constexpr pair_impl_base() = default;

    // 值构造函数
    template <typename U1, typename U2>
    constexpr pair_impl_base(U1&& u1, U2&& u2) : T1(std::forward<U1>(u1)),
                                                 second_(std::forward<U2>(u2)) {}

    // 拷贝构造函数
    constexpr pair_impl_base(const pair_impl_base&) = default;

    // 移动构造函数
    constexpr pair_impl_base(pair_impl_base&&) noexcept = default;

    // 析构函数
    ~pair_impl_base() = default;

    // 访问成员
    constexpr T1& first () noexcept { return *this; }
    constexpr T2& second() noexcept { return second_; }

    constexpr const T1& first() const noexcept { return *this; }
    constexpr const T2& second() const noexcept { return second_; }

    // 交换操作
    void swap(pair_impl_base& other) noexcept(is_nothrow_swappable_v<T1> && is_nothrow_swappable_v<T2>) {
        using std::swap;
        swap(static_cast<T1&>(*this), static_cast<T1&>(other));
        swap(second_, other.second_);
    }
};

// ============================================================================
// 情况3: 只有第二个类型是空类且非final (启用EBCO)
// ============================================================================

template <typename T1, typename T2>
class pair_impl_base<T1, T2, false, true> : private T2 {
protected:
    T1 first_;

public:
    // 默认构造函数
    constexpr pair_impl_base() = default;

    // 值构造函数
    template <typename U1, typename U2>
    constexpr pair_impl_base(U1&& u1, U2&& u2) : first_(std::forward<U1>(u1)),
                                                 T2(std::forward<U2>(u2)) {}

    // 拷贝构造函数
    constexpr pair_impl_base(const pair_impl_base&) = default;

    // 移动构造函数
    constexpr pair_impl_base(pair_impl_base&&) noexcept = default;

    // 析构函数
    ~pair_impl_base() = default;

    // 访问成员
    constexpr T1& first () noexcept { return first_; }
    constexpr T2& second() noexcept { return *this; }

    constexpr const T1& first() const noexcept { return first_; }
    constexpr const T2& second() const noexcept { return *this; }

    // 交换操作
    void swap(pair_impl_base& other) noexcept(is_nothrow_swappable_v<T1> && is_nothrow_swappable_v<T2>) {
        using std::swap;
        swap(first_, other.first_);
        swap(static_cast<T2&>(*this), static_cast<T2&>(other));
    }
};

// ============================================================================
// 情况4: 两个类型都是空类且非final (启用EBCO)
// ============================================================================

template <typename T1, typename T2>
class pair_impl_base<T1, T2, true, true> : private T1, private T2 {
public:
    // 默认构造函数
    constexpr pair_impl_base() = default;

    // 值构造函数
    template <typename U1, typename U2>
    constexpr pair_impl_base(U1&& u1, U2&& u2) : T1(std::forward<U1>(u1)),
                                                 T2(std::forward<U2>(u2)) {}

    // 拷贝构造函数
    constexpr pair_impl_base(const pair_impl_base&) = default;

    // 移动构造函数
    constexpr pair_impl_base(pair_impl_base&&) noexcept = default;

    // 析构函数
    ~pair_impl_base() = default;

    // 访问成员
    constexpr T1& first () noexcept { return *this; }
    constexpr T2& second() noexcept { return *this; }

    constexpr const T1& first() const noexcept { return *this; }
    constexpr const T2& second() const noexcept { return *this; }

    // 交换操作
    void swap(pair_impl_base& other) noexcept(is_nothrow_swappable_v<T1> && is_nothrow_swappable_v<T2>) {
        using std::swap;
        swap(static_cast<T1&>(*this), static_cast<T1&>(other));
        swap(static_cast<T2&>(*this), static_cast<T2&>(other));
    }
};

// 统一的 pair_impl 接口
template <typename T1, typename T2> 
class pair_impl : public pair_impl_base<T1, T2, is_empty_and_non_final_v<T1>, is_empty_and_non_final_v<T2>> {
private:
    using base_type = pair_impl_base<T1, T2, is_empty_and_non_final_v<T1>, is_empty_and_non_final_v<T2>>;

public:
    // 继承所有构造函数
    using base_type::base_type;

    // 默认构造函数
    constexpr pair_impl() = default;

    // 拷贝构造函数
    constexpr pair_impl(const pair_impl&) = default;

    // 移动构造函数
    constexpr pair_impl(pair_impl&&) noexcept = default;

    // 析构函数
    ~pair_impl() = default;

    // 拷贝赋值运算符
    constexpr pair_impl& operator=(const pair_impl& other) {
        if (this != &other) {
            this->first() = other.first();
            this->second() = other.second();
        }
        return *this;
    }

    // 移动赋值运算符
    constexpr pair_impl& operator=(pair_impl&& other) noexcept(is_nothrow_move_v<T1> && is_nothrow_move_v<T2>) {
        if (this != &other) {
            this->first() = std::move(other.first());
            this->second() = std::move(other.second());
        }
        return *this;
    }

    // 从其他类型赋值
    template <typename U1, typename U2> 
    constexpr pair_impl& operator=(pair_impl<U1, U2>& other) {
        this->first() = other.first();
        this->second() = other.second();
        return *this;
    }

    template <typename U1, typename U2> 
    constexpr pair_impl& operator=(pair_impl<U1, U2>&& other) {
        this->first() = std::forward<U1>(other.first());
            this->second() = std::forward<U2>(other.second());
        return *this;
    }

    // 交换操作
    void swap(pair_impl& other) noexcept(is_nothrow_swappable_v<T1> && is_nothrow_swappable_v<T2>) {
        base_type::swap(other);
    }

    // 访问成员 - 转发到基类
    using base_type::first;
    using base_type::second;
}; 

// ============================================================================
// 特化用于引用类型 (防止EBCO错误应用)
// ============================================================================
template <typename T1, typename T2> 
class pair_impl<T1&, T2> : public pair_impl_base<T1&, T2, false, false> {
    using pair_impl_base<T1&, T2, false, false>::pair_impl_base;
};

template <typename T1, typename T2> 
class pair_impl<T1, T2&> : public pair_impl_base<T1, T2&, false, false> {
    using pair_impl_base<T1, T2&, false, false>::pair_impl_base;
};

template <typename T1, typename T2> 
class pair_impl<T1&, T2&> : public pair_impl_base<T1&, T2&, false, false> {
    using pair_impl_base<T1&, T2&, false, false>::pair_impl_base;
};

// ============================================================================
// 特化用于数组类型 (需要特殊处理)
// ============================================================================

template <typename T1, typename T2, std::size_t N1, std::size_t N2> 
class pair_impl<T1[N1], T2[N2]> : public pair_impl_base<T1*, T2*, false, false> {
public:
    // 禁用数组的pair实现，因为数组不能直接作为成员
    static_assert(sizeof(T1) == 0, "Arrays cannot be used as pair members directly.");
};

} // namespace my_stl::detail

