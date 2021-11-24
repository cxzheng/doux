#pragma once

#include <complex>
#include <concepts>
#include <variant>

#include "doux/doux.h"

NAMESPACE_BEGIN(doux)

// This is purposely an incomplete type to produce compile error for inspecting 
// the deduced template parameter type.
template <typename T> struct deduced_type;

template <class T_>
struct is_complex_or_floating_point : std::is_floating_point<T_> {};

template <class T_>
struct is_complex_or_floating_point<std::complex<T_>>
    : std::is_floating_point<T_> {};

template <class T_>
inline constexpr bool is_complex_or_floating_point_v =
    is_complex_or_floating_point<T_>::value;

// -----------------------------------------------------------------------

/// Concstraits for initializer list of vector-types (such as SVector, Point,
/// etc.)
template <size_t D_, typename... Ts>
concept IsValidVecInitList =
    sizeof...(Ts) == D_&& std::conjunction_v<std::is_trivially_copyable<Ts>...>;

// -----------------------------------------------------------------------

template <typename T>
struct is_variant : std::false_type {};

template <typename... Args>
struct is_variant<std::variant<Args...>> : std::true_type {};

template <typename T>
inline constexpr bool is_variant_v = is_variant<T>::value;

template <typename T>
concept IsVariant = is_variant_v<T>;

// -----------------------------------------------------------------------
template <class...> constexpr std::false_type always_false{};
template <class...> constexpr std::true_type always_true{};

NAMESPACE_END(doux)

/*
namespace internal {

template <typename AlwaysVoid, typename... Ts>
struct has_common_type_impl : std::false_type {};

template <typename... Ts>
struct has_common_type_impl<std::void_t<std::common_type_t<Ts...>>, Ts...>
    : std::true_type {};

}  // namespace internal

template <typename... Ts>
inline constexpr bool has_common_type_v =
    internal::has_common_type_impl<void, Ts...>::value;
*/