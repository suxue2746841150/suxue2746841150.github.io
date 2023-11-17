// clib personal header
// Library reference: http://clib.awiki.org/
// This code library has only been tested for compilation on MSVC.

/**********************************
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
|-=   Author: Zhao Mengfu       =-|
|=-   Version: 2.4-23.1117(a)   -=|
|-=   Compiler: MSVC 19.37      =-|
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
**********************************/

#pragma once
#ifndef _CLIB_HPP_
#define _CLIB_HPP_
#define _DEFINE_LIBRARY_NAMESPACE       clib       /* Global Namespace */
#if defined(_BUILD_CLIB_MODULE)
#define _EXPORT_CLIB                    export
import std;
#else
#define _EXPORT_CLIB
#include <algorithm>
#include <concepts>
#include <cstdint>
#include <format>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#endif // defined(_BUILD_CLIB_MODULE)
#ifndef _STD
#define _STD                            ::std::
#endif // _STD
#ifndef _STD_BEGIN
#define _STD_BEGIN                      namespace std {
#endif // _STD_BEGIN
#ifndef _STD_END
#define _STD_END                        }
#endif // _STD_END
#define _CLIB_BEGIN                     namespace _DEFINE_LIBRARY_NAMESPACE {
#define _CLIB_END                       }
#define _MATRIX_UTILITY_BEGIN           _EXPORT_CLIB namespace matrix_utility {
#define _MATRIX_UTILITY_END             }
#define _CLIB_PRIVATE_UTILITY_BEGIN     namespace _DEFINE_LIBRARY_NAMESPACE::private_utility {
#define _CLIB_PRIVATE_UTILITY_END       }
#define _CLIB_MATRIX_UTILITY_BEGIN      namespace _DEFINE_LIBRARY_NAMESPACE::matrix_utility {
#define _CLIB_MATRIX_UTILITY_END        }
#define _NODISCARD                      [[nodiscard]]
#define _CP_UTILITY                     _DEFINE_LIBRARY_NAMESPACE::private_utility::
#ifndef _NORETURN
#define _NORETURN                       [[noreturn]]
#endif // _NORETURN
#define _CLIB_CONCEPTS_BEGIN            namespace concepts {
#define _CLIB_CONCEPTS_END              }
#define _CP_CONCEPTS                    _CP_UTILITY concepts::
#define _CHAIN_UTILITY_BEGIN            _EXPORT_CLIB namespace _DEFINE_LIBRARY_NAMESPACE::chain_utility {
#define _CHAIN_UTILITY_END              }
#if !defined(_BUILD_CLIB_MODULE)
_STD_BEGIN
// Forward declaration for specialized formatter template
template <typename, size_t>                                 class array;
template <typename, typename>                               class vector;
template <typename, typename>                               class deque;
template <typename, typename>                               class forward_list;
template <typename, typename>                               class list;
template <typename, typename, typename>                     class set;
template <typename, typename, typename>                     class multiset;
template <typename, typename, typename, typename>           class map;
template <typename, typename, typename, typename>           class multimap;
template <typename, typename, typename, typename>           class unordered_set;
template <typename, typename, typename, typename>           class unordered_multiset;
template <typename, typename, typename, typename, typename> class unordered_map;
template <typename, typename, typename, typename, typename> class unordered_multimap;
template <typename, typename>                               struct pair;
template <typename...>                                      class tuple;
template <typename, size_t>                                 class span;
template <typename, typename, typename, typename>           class mdspan;
_STD_END
#endif // defined(_BUILD_CLIB_MODULE)

_CLIB_PRIVATE_UTILITY_BEGIN
_CLIB_CONCEPTS_BEGIN
template <typename _Type>
concept _Iterable_Type = requires (_Type __val) {
    { __val.begin() } -> _STD same_as<typename _Type::iterator>;
    { __val.end() } -> _STD same_as<typename _Type::iterator>;
};

template <typename _Cont>
concept _Matrix_Type = requires (_Cont __matrix) {
    requires _STD ranges::sized_range<_Cont>;
    { __matrix.size() } -> _STD same_as<size_t>;
    requires requires (size_t __row, size_t __col) {
        requires _STD ranges::sized_range<decltype(__matrix[__row])>;
        { __matrix[__row].size() } -> _STD same_as<size_t>;
        __matrix[__row].size() == __matrix.front().size();
        _STD is_arithmetic_v<decltype(__matrix[__row][__col])>;
        { __matrix[__row][__col] } -> _STD convertible_to<long double>;
    };
};
_CLIB_CONCEPTS_END

_NODISCARD constexpr _STD string _format_container(auto _Beg, auto _End) noexcept {
    _STD stringstream _Rst;
    while (_Beg != _End) {
        _Rst << _STD format("{}", *_Beg++);
        if (_Beg != _End) { _Rst << ", "; }
    }
    return _Rst.str();
}

template<size_t... _Args>
_NODISCARD constexpr _STD string _format_tuple(const auto& __t, _STD index_sequence<_Args...>) noexcept {
    _STD stringstream _Rst;
    ((_Rst << (_Args ? ", " : "") << _STD format("{}", _STD get<_Args>(__t))), ...);
    return _Rst.str();
}
_CLIB_PRIVATE_UTILITY_END // namespace private_utility

_STD_BEGIN
// Template specialization for containers formatter
_EXPORT_CLIB template <typename _Ty, size_t _Size>
struct formatter<array<_Ty, _Size>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    constexpr auto format(const array<_Ty, _Size>& __v, _Fmt_Cont& __ctx) const {
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

_EXPORT_CLIB template <typename _Seque>
    requires _STD same_as<_Seque, _STD vector<typename _Seque::value_type, typename _Seque::allocator_type>>
          || _STD same_as<_Seque, _STD deque<typename _Seque::value_type, typename _Seque::allocator_type>>
          || _STD same_as<_Seque, _STD forward_list<typename _Seque::value_type, typename _Seque::allocator_type>>
          || _STD same_as<_Seque, _STD list<typename _Seque::value_type, typename _Seque::allocator_type>>
struct formatter<_Seque> : formatter<string_view> {
    template <typename _Fmt_Cont>
    constexpr auto format(const _Seque& __v, _Fmt_Cont& __ctx) const {
        return formatter<string_view>::format(_STD format("[{}]", _CP_UTILITY _format_container(__v.begin(), __v.end())), __ctx);
    }
};

_EXPORT_CLIB template <typename _Assoc>
    requires requires (_Assoc __cont) {
        requires requires {
            { __cont.key_comp() } -> _STD same_as<typename _Assoc::key_compare>;
            { __cont.value_comp() } -> _STD same_as<typename _Assoc::value_compare>;
        } || requires {
            { __cont.key_eq() } -> _STD same_as<typename _Assoc::key_equal>;
            { __cont.hash_function() } -> _STD same_as<typename _Assoc::hasher>;
        };
    }
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

_CHAIN_UTILITY_BEGIN // ^^^ Unstable utility / Unsafe templates vvv
// Immutable range operations
_EXPORT_CLIB template <_CP_CONCEPTS _Iterable_Type _Cont, typename... _Args>
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&> && ...)
_NODISCARD constexpr _Cont operator|(_Cont _Val, _Args&&... _Func) {
    _STD ranges::for_each(_Val, [&](auto&& _Ele) { (_Func(_Ele), ...); });
    return _Val;
}

// Mutable range operations
_EXPORT_CLIB template <_CP_CONCEPTS _Iterable_Type _Cont, typename... _Args>
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&> && ...)
constexpr _Cont& operator/(_Cont& _Val, _Args&&... _Func) {
    _STD ranges::for_each(_Val, [&](auto&& _Ele) { (_Func(_Ele), ...); });
    return _Val;
}
_CHAIN_UTILITY_END

_CLIB_BEGIN
_EXPORT_CLIB template <_CP_CONCEPTS _Matrix_Type _Cont>
_NODISCARD constexpr long double determinant(const _Cont& _Matrix) {
    using value_type = typename _Cont::value_type;
    if (_Matrix.size() != _Matrix[0].size()) { throw _STD runtime_error("Matrix must be square (number of rows must be equal to the number of columns)."); }
    switch (_Matrix.size()) {
    case 1:
        return _Matrix[0][0];
    case 2:
        return _Matrix[0][0] * _Matrix[1][1] - _Matrix[0][1] * _Matrix[1][0];
    default:
        long double __det{};
        for (size_t __loop{}; __loop < _Matrix.size(); ++__loop) {
            _Cont __sub(_Matrix.size() - 1, typename _Cont::value_type(_Matrix.size() - 1));
            for (size_t __row{ 1 }; __row < _Matrix.size(); ++__row) {
                for (size_t __col{}, __loc{}; __col < _Matrix.size(); ++__col) {
                    if (__col != __loop) {
                        __sub[__row - 1][__loc++] = _Matrix[__row][__col];
                    }
                }
            }
            __det += (__loop % 2 ? -1 : 1) * _Matrix[0][__loop] * determinant(__sub);
        }
        return __det;
    }
}

_EXPORT_CLIB template <_CP_CONCEPTS _Matrix_Type _Cont>
_NODISCARD constexpr _Cont transpose(const _Cont& _Matrix) {
    if (_Matrix.empty() || _Matrix.front().empty()) { throw _STD runtime_error("Cannot transpose an empty matrix"); }
    _Cont _Rst(_Matrix.front().size(), typename _Cont::value_type(_Matrix.size()));
    for (size_t __row{}; __row < _Matrix.size(); ++__row) {
        for (size_t __col{}; __col < _Matrix.front().size(); ++__col) {
            _Rst[__col][__row] = _Matrix[__row][__col];
        }
    }
    return _Rst;
}

_MATRIX_UTILITY_BEGIN
_EXPORT_CLIB template <_CP_CONCEPTS _Matrix_Type _Cont1, _CP_CONCEPTS _Matrix_Type _Cont2>
_NODISCARD constexpr _Cont1 operator*(const _Cont1& _Left, const _Cont2& _Right) {
    if (_Left.front().size() != _Right.size() || _Left.size() != _Right.front().size()) { throw std::invalid_argument("Matrix dimensions are not compatible for multiplication"); }
    _Cont1 _Rst(_Left.size(), typename _Cont1::value_type(_Right.front().size()));
    for (std::size_t __row_r{}; __row_r < _Left.size(); ++__row_r) {
        for (std::size_t __row_c{}; __row_c < _Right.front().size(); ++__row_c) {
            for (std::size_t __col{}; __col < _Right.size(); ++__col) {
                _Rst[__row_r][__row_c] += _Left[__row_r][__col] * _Right[__col][__row_c];
            }
        }
    }
    return _Rst;
}

_EXPORT_CLIB template <_CP_CONCEPTS _Matrix_Type _Cont>
_NODISCARD constexpr _Cont operator*(const _Cont& _Left, long double _Right) {
    _Cont _Rst(_Left);
    _STD ranges::for_each(_Rst, [_Right](auto& __row) {
        _STD ranges::for_each(__row, [_Right](auto& __col) {
            __col *= _Right;
        });
    });
    return _Rst;
}

_EXPORT_CLIB template <_CP_CONCEPTS _Matrix_Type _Cont>
_NODISCARD constexpr _Cont operator*(long double _Left, const _Cont& _Right) {
    return _Right * _Left;
}
_MATRIX_UTILITY_END

_EXPORT_CLIB template <_CP_CONCEPTS _Matrix_Type _Cont>
_NODISCARD _STD string matrix_view(const _Cont& _Matrix, size_t _Precision = 6) {
    _STD stringstream _Rst;
    size_t _size{}, _End{};
    _STD ranges::for_each(_Matrix, [&_size, _Precision](const auto& __iter) {
        _STD ranges::for_each(__iter, [&_size, _Precision](const auto& __it) {
            _STD string _temp{ _STD format("{0:.{1}f}", static_cast<long double>(__it), _Precision) };
            if (_size < _temp.size()) { _size = _temp.size(); }
        });
    });
    _size += 2;
    _STD ranges::for_each(_Matrix, [_size, _Precision, &_Rst, &_Matrix, &_End](const auto& __ele) {
        ++_End;
        _Rst << "|";
        bool _Begin{ true };
        _STD ranges::for_each(__ele, [_size, _Precision, &_Begin, &_Rst](const auto& __e) {
            size_t __space{ _size };
            if (_Begin) {
                _Begin = false;
                __space -= 2;
            }
            _Rst << _STD format("{0:>{1}.{2}f}", static_cast<long double>(__e), __space, _Precision);
        });
        _Rst << "|";
        if (_End != _Matrix.size()) { _Rst << "\n"; }
    });
    return _Rst.str();
}
_CLIB_END

_CLIB_MATRIX_UTILITY_BEGIN
_EXPORT_CLIB template <_CP_CONCEPTS _Matrix_Type _Cont1, _CP_CONCEPTS _Matrix_Type _Cont2>
_NODISCARD constexpr _Cont1 operator+(const _Cont1& _Left, const _Cont2& _Right) {
    if (_Left.size() != _Right.size() || _Left[0].size() != _Right[0].size()) { throw _STD runtime_error("Matrix dimensions are not compatible for addition"); }
    _Cont1 _Rst(_Left);
    for (size_t __row{}; __row < _Rst.size(); ++__row) {
        for (size_t __col{}; __col < _Rst[__row].size(); ++__col) {
            _Rst[__row][__col] += _Right[__row][__col];
        }
    }
    return _Rst;
}

_EXPORT_CLIB template <_CP_CONCEPTS _Matrix_Type _Cont1, _CP_CONCEPTS _Matrix_Type _Cont2>
_NODISCARD constexpr _Cont1 operator-(const _Cont1& _Left, const _Cont2& _Right) {
    if (_Left.size() != _Right.size() || _Left[0].size() != _Right[0].size()) { throw _STD runtime_error("Matrix dimensions are not compatible for subtraction"); }
    _Cont1 _Rst(_Left);
    for (size_t __row{}; __row < _Rst.size(); ++__row) {
        for (size_t __col{}; __col < _Rst[__row].size(); ++__col) {
            _Rst[__row][__col] -= _Right[__row][__col];
        }
    }
    return _Rst;
}
_CLIB_MATRIX_UTILITY_END

#undef _EXPORT_CLIB
#undef _CLIB_BEGIN
#undef _CLIB_END
#undef _CLIB_PRIVATE_UTILITY_BEGIN
#undef _CLIB_PRIVATE_UTILITY_END
#undef _CP_UTILITY
#undef _MATRIX_UTILITY_BEGIN
#undef _MATRIX_UTILITY_END
#undef _CLIB_MATRIX_UTILITY_BEGIN
#undef _CLIB_MATRIX_UTILITY_END
#undef _CLIB_CONCEPTS_BEGIN
#undef _CLIB_CONCEPTS_END
#undef _CP_CONCEPTS
#undef _CHAIN_UTILITY_BEGIN
#undef _CHAIN_UTILITY_END
#undef _DEFINE_LIBRARY_NAMESPACE
#endif // _CLIB_
