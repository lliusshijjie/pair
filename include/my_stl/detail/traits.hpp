/*
    关键特性说明
    1. EBCO 支持
        is_empty_and_non_final_v 核心特征，决定是否应用空基类优化
        排除 final 类，因为它们不能作为基类

    2. SFINAE 友好
        提供简单的 enable_if_t 替代标准库版本
        支持模板参数的条件启用

    3. 值类别保持
        remove_cvref_t 用于完美转发中的类型处理
        保持移动语义的正确性

    4. 编译时优化
        使用 inline constexpr 变量，避免ODR问题
        所有特征都在编译时计算

    5. 异常安全
        提供移动操作的 noexcept 检查特征
        支持异常安全保证的实现
*/

#pragma once

#include <type_traits>
#include <utility>

namespace my_stl {
    template <typename T1, typename T2> class pair;
}

namespace my_stl::detail {

// ============================================================================
// 基础类型特征
// ============================================================================

// 判断类型是否为空且非final （用于EBCO）
template <typename T>
struct is_empty_and_non_final : std::bool_constant<
    std::is_empty_v<T> && !std::is_final_v<T>
> {};

template <typename T>
inline constexpr bool is_empty_and_non_final_v = is_empty_and_non_final<T>::value;

// 判断类型是否可以交换
template <typename T>
struct is_swappable : std::bool_constant <
    std::is_move_constructible_v<T> && std::is_move_assignable_v<T>
> {};

template <typename T>
inline constexpr bool is_swappable_v = is_swappable<T>::value;

// 判断两个类型是否可以交换
template <typename T, typename U>
struct is_swappable_with : std::bool_constant<
    is_swappable_v<T> && is_swappable_v<U> &&
    std::is_assignable_v<T&, U&&> &&
    std::is_assignable_v<U&, T&&>
> {};

template <typename T, typename U>
inline constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;

// 判断类型是否可以无异常交换
template <typename T>
struct is_nothrow_swappable : std::bool_constant<
    is_swappable_v<T> &&
    std::is_nothrow_move_constructible_v<T> &&
    std::is_nothrow_move_assignable_v<T>
> {};

template <typename T>
inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

// ============================================================================
// 引用包装工具（用于处理引用类型）
// ============================================================================

// 移除引用和const/volatile限定符
template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template<class T>
struct unwrap_reference { using type = T; };

template<class T>
struct unwrap_reference<std::reference_wrapper<T>> { using type = T&; };

template<class T>
using unwrap_reference_t = typename unwrap_reference<T>::type;

template<class T>
using unwrap_ref_decay_t = unwrap_reference_t<std::decay_t<T>>;

// 添加引用（保持值的类型）
template <typename T>
constexpr T&& forward(std::remove_reference_t<T>& t) noexcept { 
    return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept { 
    static_assert(!std::is_lvalue_reference_v<T>, "Cannot forward an rvalue as an lvalue");
    return static_cast<T&&>(t);
}

// 移动语义支持
template <typename T>
constexpr std::remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(t);
}

// ============================================================================
// 条件类型选择
// ============================================================================

// 根据条件选择类型（类似std::conditional 但更简洁）
template <bool Condition , typename T, typename F>
struct conditional { using type = T; };

template <typename T, typename F>
struct conditional<false, T, F> { using type = F; };

template <bool Condition, typename T, typename F>
using conditional_t = typename conditional<Condition, T, F>::type;

// ============================================================================
// 启用/禁用SFINAE工具
// ============================================================================

// 简单的 enable_if 替代品
template <bool Condition, typename T = void>
struct enable_if { using type = T; };

template <typename T>
struct enable_if<true, T> { using type = T; };

template <bool Condition, typename T = void> 
using enable_if_t = typename enable_if<Condition, T>::type;

// ============================================================================
// 配对类型特征
// ============================================================================

// 判断是否为 pair 类型（前向声明）
template <typename>
struct is_pair : std::false_type {};

template <typename T1, typename T2>
struct is_pair<my_stl::pair<T1, T2>> : std::true_type {};

template <typename T>
inline constexpr bool is_pair_v = is_pair<T>::value;

// 从 pair 类型提取第一个类型
template <typename Pair>
struct pair_first;

template <typename T1, typename T2>
struct pair_first<my_stl::pair<T1, T2>> { using type = T1; };

template <typename Pair>
using pair_first_t = typename pair_first<Pair>::type;

// 从 pair 类型提取第二个类型
template <typename Pair>
struct pair_second;

template <typename T1, typename T2>
struct pair_second<my_stl::pair<T1, T2>> { using type = T2; };

template <typename Pair>
using pair_second_t = typename pair_second<Pair>::type;

// ============================================================================
// 构造和赋值约束检查
// ============================================================================

// 检查是否可以从U构造T
template <typename T, typename U>
struct is_constructible_from : std::is_constructible<T, U> {};

template <typename T, typename U>
inline constexpr bool is_constructible_from_v = is_constructible_from<T, U>::value;

// 检查是否可以从U赋值给T
template <typename T, typename U>
struct is_assignable_from : std::is_assignable<T, U> {};

template <typename T, typename U>
inline constexpr bool is_assignable_from_v = is_assignable_from<T, U>::value;

// ============================================================================
// 移动语义支持检查
// ============================================================================

// 检测类型是否有不抛异常的移动操作
template <typename T>
struct is_nothrow_move : std::bool_constant<
    std::is_nothrow_move_constructible_v<T> &&
    std::is_nothrow_move_assignable_v<T>
> {};

template <typename T>
inline constexpr bool is_nothrow_move_v = is_nothrow_move<T>::value;

// ============================================================================
// 简单的概念模拟（C++20之前）
// ============================================================================

// 可默认构造
template <typename T>
constexpr bool DefaultConstructible = std::is_default_constructible_v<T>;

// 可拷贝构造
template <typename T>
constexpr bool CopyConstructible = std::is_copy_constructible_v<T>;

// 可移动构造
template <typename T>
constexpr bool MoveConstructible = std::is_move_constructible_v<T>;

// 可拷贝赋值
template <typename T>
constexpr bool CopyAssignable = std::is_copy_assignable_v<T>;

// 可移动赋值
template <typename T>
constexpr bool MoveAssignable = std::is_move_assignable_v<T>;

// ============================================================================
// 编译时常量工具
// ============================================================================

// 简单的整数序列（用于索引展开）
template <std::size_t... Ints>
struct index_sequence {};

template <std::size_t N, std::size_t... Ints>
struct make_index_sequence_helper : make_index_sequence_helper<N - 1, N - 1, Ints...> {};

template <std::size_t... Ints>
struct make_index_sequence_helper<0, Ints...> {
    using type = index_sequence<Ints...>;
};

template <std::size_t N>
using make_index_sequence = typename make_index_sequence_helper<N>::type;

// ============================================================================
// 类型标签分发
// ============================================================================

// 简单的类型标签
struct empty_type_tag {};
struct non_empty_type_tag {};

// 根据是否为空类型选择标签
template <typename T>
using type_category = conditional_t<
    is_empty_and_non_final_v<T>,
    empty_type_tag,
    non_empty_type_tag
>;

} // namespace my_stl::detail














