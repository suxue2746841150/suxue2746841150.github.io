// clib personal header

/**************************************************
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
|-  Author: Zhao Mengfu                          -|
|=  Version: 1.4-23.1109(c)                      =|
|-  Compiler: Microsoft Visual C++ 2022 v17.7.6  -|
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
**************************************************/

#pragma once
#ifndef _CLIB_HPP_
#define _CLIB_HPP_

#include <yvals_core.h>
#define _EXPORT_CLIB_STD _EXPORT_STD
#define _STD             ::std::

#if _HAS_CXX23 && defined(_BUILD_STD_MODULE)
#define _EXPORT_CLIB export
#define _EXPORT_CLIB_PRIVATE_UTILITY export
#else
#define _EXPORT_CLIB
#define _EXPORT_CLIB_PRIVATE_UTILITY
#endif // _HAS_CXX23 && defined(_BUILD_STD_MODULE)

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <type_traits>
#if _HAS_CXX17
#include <string_view>
#endif // _HAS_CXX17
#if _HAS_CXX20
#include <concepts>
#include <format>
#include <ranges>
#endif // _HAS_CXX20

#define _CLIB_BEGIN namespace clib {
#define _CLIB_END   }
#define _CLIB_PRIVATE_UTILITY_BEGIN namespace clib::private_utility {
#define _CLIB_PRIVATE_UTILITY_END   }
#define _CP_UTILITY clib::private_utility::
#define _NORETURN   [[noreturn]]
#if _HAS_CXX17
#define _NODISCARD  [[nodiscard]]
#else
#define _NODISCARD
#endif // _HAS_CXX17

#if _HAS_CXX20
_STD_BEGIN

// Forward declaration for specialized formatter template
_EXPORT_CLIB_STD template <typename, size_t>                                 class array;
_EXPORT_CLIB_STD template <typename, typename>                               class vector;
_EXPORT_CLIB_STD template <typename, typename>                               class deque;
_EXPORT_CLIB_STD template <typename, typename>                               class forward_list;
_EXPORT_CLIB_STD template <typename, typename>                               class list;
_EXPORT_CLIB_STD template <typename, typename, typename>                     class set;
_EXPORT_CLIB_STD template <typename, typename, typename>                     class multiset;
_EXPORT_CLIB_STD template <typename, typename, typename, typename>           class map;
_EXPORT_CLIB_STD template <typename, typename, typename, typename>           class multimap;
_EXPORT_CLIB_STD template <typename, typename, typename, typename>           class unordered_set;
_EXPORT_CLIB_STD template <typename, typename, typename, typename>           class unordered_multiset;
_EXPORT_CLIB_STD template <typename, typename, typename, typename, typename> class unordered_map;
_EXPORT_CLIB_STD template <typename, typename, typename, typename, typename> class unordered_multimap;
_EXPORT_CLIB_STD template <typename, typename>                               struct pair;
_EXPORT_CLIB_STD template <typename...>                                      class tuple;
_EXPORT_CLIB_STD template <typename, size_t>                                 class span;

#if _HAS_CXX23
// ^^^ Undefinded class
_EXPORT_CLIB_STD template <typename, typename, typename, typename>           class mdspan;
#endif // _HASCXX_23

_STD_END
#endif // _HAS_CXX20

_CLIB_PRIVATE_UTILITY_BEGIN

_EXPORT_CLIB_PRIVATE_UTILITY template <typename> struct _Has_Iterator;
#if _HAS_CXX20
_EXPORT_CLIB_PRIVATE_UTILITY template <typename _Type>
concept _Iterable_Type = requires (const _Type& _Val) {
    _Val.begin();
    _Val.end();
};

_EXPORT_CLIB_PRIVATE_UTILITY template <typename _Cont>
concept _Seque_Cont = (
    _STD same_as<_Cont, _STD vector<typename _Cont::value_type, typename _Cont::allocator_type>>       ||
    _STD same_as<_Cont, _STD deque<typename _Cont::value_type, typename _Cont::allocator_type>>        ||
    _STD same_as<_Cont, _STD forward_list<typename _Cont::value_type, typename _Cont::allocator_type>> ||
    _STD same_as<_Cont, _STD list<typename _Cont::value_type, typename _Cont::allocator_type>>
);

_EXPORT_CLIB_PRIVATE_UTILITY template <typename _Or_Assoc>
concept _Or_Assoc_Cont = requires (_Or_Assoc _A) {
    { _A.key_comp() } -> _STD same_as<typename _Or_Assoc::key_compare>;
    { _A.value_comp() } -> _STD same_as<typename _Or_Assoc::value_compare>;
};

_EXPORT_CLIB_PRIVATE_UTILITY template <typename _Un_Assoc>
concept _Un_Assoc_Cont = requires (_Un_Assoc _U) {
    { _U.key_eq() } -> _STD same_as<typename _Un_Assoc::key_equal>;
    { _U.hash_function() } -> _STD same_as<typename _Un_Assoc::hasher>;
};

_EXPORT_CLIB_PRIVATE_UTILITY template <typename _Assoc>
concept _Assoc_Cont = _Or_Assoc_Cont<_Assoc> || _Un_Assoc_Cont<_Assoc>;
#endif // _HAS_CXX20

#if _HAS_CXX20
_EXPORT_CLIB_PRIVATE_UTILITY template <typename _Iter>
_NODISCARD constexpr _STD string _format_container(_Iter, _Iter) noexcept;

_EXPORT_CLIB_PRIVATE_UTILITY template<typename _Tp, size_t... _Args>
_NODISCARD constexpr _STD string _format_tuple(const _Tp& __t, _STD index_sequence<_Args...>) noexcept;
#endif // _HAS_CXX20

_CLIB_PRIVATE_UTILITY_END // namespace private_utility

#if _HAS_CXX20
#define _STD_BEGIN namespace std {
#define _STD_END   }

_STD_BEGIN

// Template specialization for containers formatter
_EXPORT_CLIB template <typename _Ty, size_t _Size>
struct formatter<array<_Ty, _Size>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    constexpr auto format(const array<_Ty, _Size>& __v, _Fmt_Cont& __ctx) const {
        return formatter<string_view>::format(_STD format("[{}]", _CP_UTILITY _format_container(__v.begin(), __v.end())), __ctx);
    }
};

_EXPORT_CLIB template <_CP_UTILITY _Seque_Cont _Seque>
struct formatter<_Seque> : formatter<string_view> {
    template <typename _Fmt_Cont>
    constexpr auto format(const _Seque& __v, _Fmt_Cont& __ctx) const {
        return formatter<string_view>::format(_STD format("[{}]", _CP_UTILITY _format_container(__v.begin(), __v.end())), __ctx);
    }
};

_EXPORT_CLIB template <typename _Ty, size_t _Extent>
struct formatter<span<_Ty, _Extent>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    constexpr auto format(const span<_Ty, _Extent>& __v, _Fmt_Cont& __ctx) const {
        return formatter<string_view>::format(_STD format("[{}]", _CP_UTILITY _format_container(__v.begin(), __v.end())), __ctx);
    }
};

_EXPORT_CLIB template <_CP_UTILITY _Assoc_Cont _Assoc>
struct formatter<_Assoc> : formatter<string_view> {
    template <typename _Fmt_Cont>
    constexpr auto format(const _Assoc& __v, _Fmt_Cont& __ctx) const {
        return formatter<string_view>::format(_STD format("{{{}}}", _CP_UTILITY _format_container(__v.begin(), __v.end())), __ctx);
    }
};

_EXPORT_CLIB template <typename _Ty1, typename _Ty2>
struct formatter<pair<_Ty1, _Ty2>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    constexpr auto format(const pair<_Ty1, _Ty2>& __v, _Fmt_Cont& __ctx) const {
        return formatter<string_view>::format(_STD format("({}, {})", __v.first, __v.second), __ctx);
    }
};

_EXPORT_CLIB template <typename... _Rest>
struct formatter<tuple<_Rest...>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    constexpr auto format(const tuple<_Rest...>& __v, _Fmt_Cont& __ctx) const {
        return formatter<string_view>::format(_STD format("({})", _CP_UTILITY _format_tuple(__v, _STD make_index_sequence<_STD tuple_size_v<tuple<_Rest...>>>())), __ctx);
    }
};

_STD_END
#endif // _HAS_CXX20

#if _HAS_CXX20
#define _CHAIN_UTILITY_BEGIN namespace clib::chain_utility {
#define _CHAIN_UTILITY_END   }

// ^^^ Unstable utility / Unsafe templates vvv
_CHAIN_UTILITY_BEGIN

// Immutable range operations
_EXPORT_CLIB template <_CP_UTILITY _Iterable_Type _Cont, typename... _Args>
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&>&&...)
_NODISCARD constexpr _Cont operator|(_Cont _Val, _Args&&... _Func) {
    _STD ranges::for_each(_Val, [&](auto&& _Ele) { (_Func(_Ele), ...); });
    return _Val;
}

// Mutable range operations
_EXPORT_CLIB template <_CP_UTILITY _Iterable_Type _Cont, typename... _Args>
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&>&&...)
constexpr _Cont& operator/(_Cont& _Val, _Args&&... _Func) {
    _STD ranges::for_each(_Val, [&](auto&& _Ele) { (_Func(_Ele), ...); });
    return _Val;
}

_CHAIN_UTILITY_END

#undef _CHAIN_UTILITY_BEGIN
#undef _CHAIN_UTILITY_END
#endif // _HAS_CXX20

_CLIB_PRIVATE_UTILITY_BEGIN

#if _HAS_CXX20
_EXPORT_CLIB_PRIVATE_UTILITY template <typename _Iter>
_NODISCARD constexpr _STD string _format_container(_Iter _Beg, _Iter _End) noexcept {
    _STD stringstream _Rst;
    while (_Beg != _End) {
        _Rst << _STD format("{}", *_Beg++);
        if (_Beg != _End) { _Rst << ", "; }
    }
    return _Rst.str();
}

_EXPORT_CLIB_PRIVATE_UTILITY template<typename _Tp, size_t... _Args>
_NODISCARD constexpr _STD string _format_tuple(const _Tp& __t, _STD index_sequence<_Args...>) noexcept {
    _STD stringstream _Rst;
    ((_Rst << (_Args ? ", " : "") << _STD format("{}", _STD get<_Args>(__t))), ...);
    return _Rst.str();
}
#endif // _HAS_CXX20
_EXPORT_CLIB_PRIVATE_UTILITY template <typename _Type>
struct _Has_Iterator {
    template <typename _Test_Type, typename = decltype(_STD declval<_Test_Type>().begin()), typename = decltype(_STD declval<_Test_Type>().end())>
    static constexpr bool _Is_Iterable() { return true; }
    template <typename>
    static constexpr bool _Is_Iterable(...) { return false; }
    static constexpr bool value = _Is_Iterable<_Type>();
};

_CLIB_PRIVATE_UTILITY_END // namespace private_utility

#undef _EXPORT_CLIB_STD
#undef _EXPORT_CLIB_PRIVATE_UTILITY
#undef _CLIB_BEGIN
#undef _CLIB_END
#undef _CLIB_PRIVATE_UTILITY_BEGIN
#undef _CLIB_PRIVATE_UTILITY_END
#undef _CP_UTILITY
#endif // _CLIB_
