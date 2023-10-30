// clib personal header

/**************************************************
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
|-  Author: Zhao Mengfu                          -|
|=  Version: 1.2-23.1030                         =|
|-  Compiler: Microsoft Visual C++ 2022 v17.7.5  -|
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
**************************************************/

#pragma once
#ifndef _CLIB_HPP_
#define _CLIB_HPP_

#define _EXPORT_CLIB
#define _CLIB_PRIVATE_UTILITY

#include <cstdint>
#include <sstream>
#include <type_traits>

#define _NODISCARD  [[nodiscard]]
#ifndef _ARG_S_
#define _ARG_S_     ...
#endif // _ARG_S_
#define _STD_BEGIN  namespace std {
#define _STD_END    }
#define _STD        ::std::
#define _CLIB_BEGIN namespace clib {
#define _CLIB_END   }

_CLIB_BEGIN

namespace private_utility {
#ifdef _HAS_CXX20
    _CLIB_PRIVATE_UTILITY template <typename _Type>
        concept _Iterable_Type = requires (const _Type& _Val) {
        _Val.begin();
        _Val.end();
    };
#else
    _CLIB_PRIVATE_UTILITY template <typename _Type>
        struct _Has_Iterator {
        template <typename _Test_Type,
            typename = decltype(_STD declval<_Test_Type>().begin()),
            typename = decltype(_STD declval<_Test_Type>().end())>
        static constexpr bool _Is_Iterable() {
            return true;
        }
        template <typename _Test_Type>
        static constexpr bool _Is_Iterable(_ARG_S_) {
            return false;
        }
        static constexpr bool value = _Is_Iterable<_Type>();
    };
#endif // _HAS_CXX20

#ifdef _HAS_CXX20
    template <typename _Iter>
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

    template <_Iterable_Type _Type>
    _NODISCARD inline constexpr _STD string _format_sequence_containers(_Type _Val) noexcept {
        return _format_container(_Val.begin(), _Val.end(), "[", "]");
    }

    template <_Iterable_Type _Type>
    _NODISCARD inline constexpr _STD string _format_associative_containers(_Type _Val) noexcept {
        return _format_container(_Val.begin(), _Val.end(), "{", "}");
    }

    template <_Iterable_Type _Type>
    _NODISCARD inline constexpr _STD string _format_associative_pair_containers(_Type _Val) noexcept {
        _STD stringstream _Rst;
        _Rst << "{";
        auto __iter = _Val.begin();
        while (__iter != _Val.end()) {
            _Rst << _STD format("({} -> {})", __iter->first, __iter->second);
            ++__iter;
            if (__iter != _Val.end()) { _Rst << ", "; }
        }
        return _Rst.str() + "}";
    }
#endif // _HAS_CXX20
}

_CLIB_END

#ifdef _HAS_CXX20
#include <format> // Template specialization for Containers formatter
_STD_BEGIN

// Template specialization for arrays formatter
template <typename _T, size_t _Size> class array;
template <typename _T, size_t _Size>
struct formatter<array<_T, _Size>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const array<_T, _Size>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for vectors formatter
template <typename _T, typename _Alloc> class vector;
template <typename _T, typename _Alloc>
struct formatter<vector<_T, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const vector<_T, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for deques formatter
template <typename _T, typename _Alloc> class deque;
template <typename _T, typename _Alloc>
struct formatter<deque<_T, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const deque<_T, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for forward_lists formatter
template <typename _T, typename _Alloc> class forward_list;
template <typename _T, typename _Alloc>
struct formatter<forward_list<_T, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const forward_list<_T, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for lists formatter
template <typename _T, typename _Alloc> class list;
template <typename _T, typename _Alloc>
struct formatter<list<_T, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const list<_T, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_sequence_containers(__v), __ctx);
    }
};

// Template specialization for sets formatter
template <typename _T, typename _Cmp, typename _Alloc> class set;
template <typename _T, typename _Cmp, typename _Alloc>
struct formatter<set<_T, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const set<_T, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_associative_containers(__v), __ctx);
    }
};

// Template specialization for multisets formatter
template <typename _T, typename _Cmp, typename _Alloc> class multiset;
template <typename _T, typename _Cmp, typename _Alloc>
struct formatter<multiset<_T, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const multiset<_T, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_associative_containers(__v), __ctx);
    }
};

// Template specialization for maps formatter
template <typename _Key, typename _Tp, typename _Cmp, typename _Alloc> class map;
template <typename _Key, typename _Tp, typename _Cmp, typename _Alloc>
struct formatter<map<_Key, _Tp, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const map<_Key, _Tp, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_associative_pair_containers(__v), __ctx);
    }
};

// Template specialization for multimaps formatter
template <typename _Key, typename _Tp, typename _Cmp, typename _Alloc> class multimap;
template <typename _Key, typename _Tp, typename _Cmp, typename _Alloc>
struct formatter<multimap<_Key, _Tp, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const multimap<_Key, _Tp, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_associative_pair_containers(__v), __ctx);
    }
};

// Template specialization for unordered_sets formatter
template <typename _T, typename _Hash, typename _Pred, typename _Alloc> class unordered_set;
template <typename _T, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_set<_T, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const unordered_set<_T, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_associative_containers(__v), __ctx);
    }
};

// Template specialization for unordered_multisets formatter
template <typename _T, typename _Hash, typename _Pred, typename _Alloc> class unordered_multiset;
template <typename _T, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_multiset<_T, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const unordered_multiset<_T, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_associative_containers(__v), __ctx);
    }
};

// Template specialization for unordered_maps formatter
template <typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc> class unordered_map;
template <typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_associative_pair_containers(__v), __ctx);
    }
};

// Template specialization for unordered_multimaps formatter
template <typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc> class unordered_multimap;
template <typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_multimap<_Key, _Tp, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    inline constexpr auto format(const unordered_multimap<_Key, _Tp, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) const noexcept {
        return formatter<string_view>::format(clib::private_utility::_format_associative_pair_containers(__v), __ctx);
    }
};

_STD_END
#endif // _HAS_CXX20

#ifdef _HAS_CXX20
#define _CHAIN_UTILITY_BEGIN namespace clib::chain_utility {
#define _CHAIN_UTILITY_END   }

_CHAIN_UTILITY_BEGIN

_EXPORT_CLIB template <clib::private_utility::_Iterable_Type _Cont, class ..._Args> // Immutable Range Operations
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&>&& _ARG_S_)
_NODISCARD inline constexpr _Cont operator|(_Cont _Val, _Args&& ..._Func) {
    for (auto&& _Ele : _Val) {
        (_Func(_Ele), _ARG_S_);
    }
    return _Val;
}

_EXPORT_CLIB template <clib::private_utility::_Iterable_Type _Cont, class ..._Args> // Mutable Range Operations
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&>&& _ARG_S_)
_NODISCARD inline constexpr _Cont& operator/(_Cont& _Val, _Args&& ..._Func) {
    for (auto&& _Ele : _Val) {
        (_Func(_Ele), _ARG_S_);
    }
    return _Val;
}

_CHAIN_UTILITY_END
#endif // _HAS_CXX20

#ifdef _ARG_S_
#undef _ARG_S_
#endif // _ARG_S_

#endif // _CLIB_
