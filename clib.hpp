// clib personal header

/**************************************************
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
|-  Author: Zhao Mengfu                          -|
|=  Version: 1.3-23.1106(a)                      =|
|-  Compiler: Microsoft Visual C++ 2022 v17.7.6  -|
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
**************************************************/

#pragma once
#ifndef _CLIB_HPP_
#define _CLIB_HPP_

#include <yvals_core.h>
#define _EXPORT_CLIB_STD _EXPORT_STD

#if _HAS_CXX23 && defined(_BUILD_STD_MODULE)
#define _EXPORT_CLIB export
#else
#define _EXPORT_CLIB
#endif

#if _HAS_CXX23 && defined(_BUILD_STD_MODULE)
#define _EXPORT_CLIB_PRIVATE_UTILITY export
#else
#define _EXPORT_CLIB_PRIVATE_UTILITY
#endif

#include <cstdint>
#include <sstream>
#include <type_traits>

#define _NORETURN   [[noreturn]]
#if _HAS_CXX17
#define _NODISCARD  [[nodiscard]]
#else
#define _NODISCARD
#endif // _HAS_CXX17
#define _STD_BEGIN  namespace std {
#define _STD_END    }
#define _STD        ::std::
#define _CLIB_BEGIN inline namespace clib {
#define _CLIB_END   }
#define _CP_UTILITY clib::private_utility::
#define _ARG_S_     ...

_CLIB_BEGIN

inline namespace private_utility {
    _EXPORT_CLIB_PRIVATE_UTILITY template <typename> struct _Has_Iterator;
#if _HAS_CXX20
    _EXPORT_CLIB_PRIVATE_UTILITY template <typename _Type>
    concept _Iterable_Type = requires (_Type&& _Val) {
        _Val.begin();
        _Val.end();
    };
#endif // _HAS_CXX20

#if _HAS_CXX20
    _EXPORT_CLIB_PRIVATE_UTILITY template <typename _Iter>
    _NODISCARD inline constexpr _STD string _format_container(_Iter, _Iter, const _STD string&, const _STD string&) noexcept;

    _EXPORT_CLIB_PRIVATE_UTILITY template <typename _Iter>
    _NODISCARD inline constexpr _STD string _format_container(_Iter, _Iter, const _STD string&, _STD string_view, _STD string_view, _STD string_view, const _STD string&) noexcept;

    _EXPORT_CLIB_PRIVATE_UTILITY template<typename _Tp, size_t ..._Args>
    _NODISCARD inline constexpr _STD string _format_tuple(const _Tp& __t, _STD index_sequence<_Args...>) noexcept;

    _EXPORT_CLIB_PRIVATE_UTILITY template <_Iterable_Type _Type>
    _NODISCARD inline constexpr _STD string _format_sequence_containers(_Type _Val) noexcept {
        return _format_container(_Val.begin(), _Val.end(), "[", "]");
    }

    _EXPORT_CLIB_PRIVATE_UTILITY template <_Iterable_Type _Type>
    _NODISCARD inline constexpr _STD string _format_associative_containers(_Type _Val) noexcept {
        return _format_container(_Val.begin(), _Val.end(), "{", "}");
    }

    _EXPORT_CLIB_PRIVATE_UTILITY template <_Iterable_Type _Type>
    _NODISCARD inline constexpr _STD string _format_associative_pair_containers(_Type _Val) noexcept {
        return _format_container(_Val.begin(), _Val.end(), "{", "(", " -> ", ")", "}");
    }
#endif // _HAS_CXX20
} // namespace private_utility

_CLIB_END

#if _HAS_CXX20
// Template specialization for Containers formatter
#include <format>
_STD_BEGIN

// Template specialization for arrays formatter
_EXPORT_CLIB_STD template <typename, size_t> class array;
_EXPORT_CLIB template <typename _Ty, size_t _Size>
struct formatter<array<_Ty, _Size>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const array<_Ty, _Size>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for vectors formatter
_EXPORT_CLIB_STD template <typename, typename> class vector;
_EXPORT_CLIB template <typename _Ty, typename _Alloc>
struct formatter<vector<_Ty, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const vector<_Ty, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for deques formatter
_EXPORT_CLIB_STD template <typename, typename> class deque;
_EXPORT_CLIB template <typename _Ty, typename _Alloc>
struct formatter<deque<_Ty, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const deque<_Ty, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for forward_lists formatter
_EXPORT_CLIB_STD template <typename, typename> class forward_list;
_EXPORT_CLIB template <typename _Ty, typename _Alloc>
struct formatter<forward_list<_Ty, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const forward_list<_Ty, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for lists formatter
_EXPORT_CLIB_STD template <typename, typename> class list;
_EXPORT_CLIB template <typename _Ty, typename _Alloc>
struct formatter<list<_Ty, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const list<_Ty, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for sets formatter
_EXPORT_CLIB_STD template <typename, typename, typename> class set;
_EXPORT_CLIB template <typename _Ty, typename _Cmp, typename _Alloc>
struct formatter<set<_Ty, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const set<_Ty, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_associative_containers(__v), __ctx);
    }
};

// Template specialization for multisets formatter
_EXPORT_CLIB_STD template <typename, typename, typename> class multiset;
_EXPORT_CLIB template <typename _Ty, typename _Cmp, typename _Alloc>
struct formatter<multiset<_Ty, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const multiset<_Ty, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_associative_containers(__v), __ctx);
    }
};

// Template specialization for maps formatter
_EXPORT_CLIB_STD template <typename, typename, typename, typename> class map;
_EXPORT_CLIB template <typename _Key, typename _Tp, typename _Cmp, typename _Alloc>
struct formatter<map<_Key, _Tp, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const map<_Key, _Tp, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_associative_pair_containers(__v), __ctx);
    }
};

// Template specialization for multimaps formatter
_EXPORT_CLIB_STD template <typename, typename, typename, typename> class multimap;
_EXPORT_CLIB template <typename _Key, typename _Tp, typename _Cmp, typename _Alloc>
struct formatter<multimap<_Key, _Tp, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const multimap<_Key, _Tp, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_associative_pair_containers(__v), __ctx);
    }
};

// Template specialization for unordered_sets formatter
_EXPORT_CLIB_STD template <typename, typename, typename, typename> class unordered_set;
_EXPORT_CLIB template <typename _Ty, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_set<_Ty, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const unordered_set<_Ty, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_associative_containers(__v), __ctx);
    }
};

// Template specialization for unordered_multisets formatter
_EXPORT_CLIB_STD template <typename, typename, typename, typename> class unordered_multiset;
_EXPORT_CLIB template <typename _Ty, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_multiset<_Ty, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const unordered_multiset<_Ty, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_associative_containers(__v), __ctx);
    }
};

// Template specialization for unordered_maps formatter
_EXPORT_CLIB_STD template <typename, typename, typename, typename, typename> class unordered_map;
_EXPORT_CLIB template <typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_associative_pair_containers(__v), __ctx);
    }
};

// Template specialization for unordered_multimaps formatter
_EXPORT_CLIB_STD template <typename, typename, typename, typename, typename> class unordered_multimap;
_EXPORT_CLIB template <typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_multimap<_Key, _Tp, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const unordered_multimap<_Key, _Tp, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_CP_UTILITY _format_associative_pair_containers(__v), __ctx);
    }
};

// Template specialization for pairs formatter
_EXPORT_CLIB_STD template <class, class> struct pair;
_EXPORT_CLIB template <class _Ty1, class _Ty2>
struct formatter<pair<_Ty1, _Ty2>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const pair<_Ty1, _Ty2>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_STD format("({}, {})", __v.first, __v.second), __ctx);
    }
};

// Template specialization for tuples formatter
_EXPORT_CLIB_STD template <class _ARG_S_> class tuple;
_EXPORT_CLIB template <class ..._Rest>
struct formatter<tuple<_Rest...>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const tuple<_Rest...>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(_STD format("({})", _CP_UTILITY _format_tuple(__v, _STD make_index_sequence<_STD tuple_size_v<tuple<_Rest...>>>())), __ctx);
    }
};

_STD_END

#endif // _HAS_CXX20

#if _HAS_CXX20
#include <algorithm>
#include <ranges>
#define _CHAIN_UTILITY_BEGIN namespace clib::chain_utility {
#define _CHAIN_UTILITY_END   }

// ^^^ UNSTABLE UTILITY / UNSAFE TEMPLATES vvv
_CHAIN_UTILITY_BEGIN

// Immutable Range Operations
_EXPORT_CLIB template <_CP_UTILITY _Iterable_Type _Cont, class ..._Args>
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&>&& _ARG_S_)
_NODISCARD inline constexpr _Cont operator|(_Cont _Val, _Args&& ..._Func) {
    _STD ranges::for_each(_Val, [&](auto&& _Ele) { (_Func(_Ele), _ARG_S_); });
    return _Val;
}

// Mutable Range Operations
_EXPORT_CLIB template <_CP_UTILITY _Iterable_Type _Cont, class ..._Args>
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&>&& _ARG_S_)
inline constexpr _Cont& operator/(_Cont& _Val, _Args&& ..._Func) {
    _STD ranges::for_each(_Val, [&](auto&& _Ele) { (_Func(_Ele), _ARG_S_); });
    return _Val;
}

_CHAIN_UTILITY_END

#undef _CHAIN_UTILITY_BEGIN
#undef _CHAIN_UTILITY_END
#endif // _HAS_CXX20

_CLIB_BEGIN

inline namespace private_utility {
#if _HAS_CXX20
    _EXPORT_CLIB_PRIVATE_UTILITY template <typename _Iter>
    _NODISCARD inline constexpr _STD string _format_container(_Iter _Beg, _Iter _End, const _STD string& _Op, const _STD string& _Ed) noexcept {
        _STD stringstream _Rst;
        _Rst << _Op;
        while (_Beg != _End) {
            _Rst << _STD format("{}", *_Beg);
            ++_Beg;
            if (_Beg != _End) { _Rst << ", "; }
        }
        return _Rst.str() + _Ed;
    }

    _EXPORT_CLIB_PRIVATE_UTILITY template <typename _Iter>
    _NODISCARD inline constexpr _STD string _format_container(_Iter _Beg, _Iter _End, const _STD string& _Op, _STD string_view _Lef, _STD string_view _Mid, _STD string_view _Rig, const _STD string& _Ed) noexcept {
        _STD stringstream _Rst;
        _Rst << _Op;
        while (_Beg != _End) {
            _Rst << _STD format("{}{}{}{}{}", _Lef, _Beg->first, _Mid, _Beg->second, _Rig);
            ++_Beg;
            if (_Beg != _End) { _Rst << ", "; }
        }
        return _Rst.str() + _Ed;
    }

    _EXPORT_CLIB_PRIVATE_UTILITY template<typename _Tp, size_t ..._Args>
    _NODISCARD inline constexpr _STD string _format_tuple(const _Tp& __t, _STD index_sequence<_Args...>) noexcept {
        _STD stringstream _Rst;
        ((_Rst << (_Args ? ", " : "") << _STD format("{}", _STD get<_Args>(__t))), _ARG_S_);
        return _Rst.str();
    }
#endif // _HAS_CXX20
    _EXPORT_CLIB_PRIVATE_UTILITY template <typename _Type>
    struct _Has_Iterator {
        template <typename _Test_Type,
            typename = decltype(_STD declval<_Test_Type>().begin()),
            typename = decltype(_STD declval<_Test_Type>().end())>
        static constexpr bool _Is_Iterable() { return true; }
        template <typename>
        static constexpr bool _Is_Iterable(_ARG_S_) { return false; }
        static constexpr bool value = _Is_Iterable<_Type>();
    };
} // namespace private_utility

_CLIB_END

#undef _EXPORT_CLIB_STD
#undef _EXPORT_CLIB_PRIVATE_UTILITY
#undef _CLIB_BEGIN
#undef _CLIB_END
#undef _CP_UTILITY
#undef _ARG_S_
#endif // _CLIB_
