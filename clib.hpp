// clib personal header

/**************************************************
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
|-  Author: Zhao Mengfu                          -|
|=  Version: 1.0.0                               =|
|-  Compiler: Microsoft Visual C++ 2022 v17.7.5  -|
|=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|
**************************************************/

#pragma once
#ifndef _CLIB_HPP_
#define _CLIB_HPP_

#define _EXPORT_CLIB
#define _CLIB_PRIVATE_UTILITY

#include <cstdint>
#include <type_traits>
#include <string>

#define _DISCARDED
#define _NODISCARD  [[nodiscard]]
#define _ARG_S_     ...
#define _STD_BEGIN  namespace std {
#define _STD_END    }
#define _STD        ::std::
#define _CLIB_BEGIN namespace clib {
#define _CLIB_END   }

_CLIB_BEGIN

namespace private_utility {
    // Temporarily discarded ^^^
    _CLIB_PRIVATE_UTILITY template <typename _Type>
    _DISCARDED class _Has_Iterator {
        template <typename _Test_Type,
            typename = decltype(_STD declval<_Test_Type>().begin()),
            typename = decltype(_STD declval<_Test_Type>().end())>
        static constexpr bool _Is_Iterable() {
            return true;
        }
        template <typename _Test_Type>
        static constexpr bool _Is_Iterable(...) {
            return false;
        }
        static constexpr bool value = _Is_Iterable<_Type>();
    };
    // vvv
}

_CLIB_END

#ifdef _HAS_CXX20
#include <format> // Template specialization for Containers formatter
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
_STD_BEGIN

// Template specialization for arrays formatter
template <typename _T, size_t _Size>
struct formatter<array<_T, _Size>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const array<_T, _Size>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "[";
        for (size_t __loc{}; __loc < __v.size(); ++__loc) {
            _Rst += _STD format("{}", __v[__loc]);
            if (__loc < __v.size() - 1) {
                _Rst += ", ";
            }
        }
        _Rst.push_back(']');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for vectors formatter
template <typename _T, typename _Alloc>
struct formatter<vector<_T, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const vector<_T, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "[";
        for (size_t __loc{}; __loc < __v.size(); ++__loc) {
            _Rst += _STD format("{}", __v[__loc]);
            if (__loc < __v.size() - 1) {
                _Rst += ", ";
            }
        }
        _Rst.push_back(']');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for deques formatter
template <typename _T, typename _Alloc>
struct formatter<deque<_T, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const deque<_T, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "[";
        for (size_t __loc{}; __loc < __v.size(); ++__loc) {
            _Rst += _STD format("{}", __v[__loc]);
            if (__loc < __v.size() - 1) {
                _Rst += ", ";
            }
        }
        _Rst.push_back(']');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for lists formatter
template <typename _T, typename _Alloc>
struct formatter<list<_T, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const list<_T, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "[";
        auto __iter = __v.begin();
        while (__iter != __v.end()) {
            _Rst += _STD format("{}", *__iter);
            ++__iter;
            if (__iter != __v.end()) {
                _Rst += ", ";
            }
        }
        _Rst.push_back(']');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for forward_lists formatter
template <typename _T, typename _Alloc>
struct formatter<forward_list<_T, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const forward_list<_T, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "[";
        auto __iter = __v.begin();
        auto __end = __v.end();
        if (__iter != __end) {
            while (true) {
                _Rst += _STD format("{}", *__iter);
                ++__iter;
                if (__iter != __end) {
                    _Rst += ", ";
                } else {
                    break;
                }
            }
        }
        _Rst.push_back(']');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for sets formatter
template <typename _T, typename _Cmp, typename _Alloc>
struct formatter<set<_T, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const set<_T, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "{";
        auto __it = __v.begin();
        while (__it != __v.end()) {
            _Rst += _STD format("{}", *__it);
            ++__it;
            if (__it != __v.end()) {
                _Rst += ", ";
            }
        }
        _Rst.push_back('}');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for maps formatter
template <typename _Key, typename _Tp, typename _Cmp, typename _Alloc>
struct formatter<map<_Key, _Tp, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const map<_Key, _Tp, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "{";
        auto __it = __v.begin();
        while (__it != __v.end()) {
            _Rst += _STD format("({} -> {})", __it->first, __it->second);
            ++__it;
            if (__it != __v.end()) {
                _Rst += ", ";
            }
        }
        _Rst.push_back('}');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for multisets formatter
template <typename _T, typename _Cmp, typename _Alloc>
struct formatter<multiset<_T, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const multiset<_T, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "{";
        auto __it = __v.begin();
        while (__it != __v.end()) {
            _Rst += _STD format("{}", *__it);
            ++__it;
            if (__it != __v.end()) {
                _Rst += ", ";
            }
        }
        _Rst.push_back('}');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for multimaps formatter
template <typename _Key, typename _Tp, typename _Cmp, typename _Alloc>
struct formatter<multimap<_Key, _Tp, _Cmp, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const multimap<_Key, _Tp, _Cmp, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "{";
        auto __it = __v.begin();
        while (__it != __v.end()) {
            _Rst += _STD format("({} -> {})", __it->first, __it->second);
            ++__it;
            if (__it != __v.end()) {
                _Rst += ", ";
            }
        }
        _Rst.push_back('}');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for unordered_sets formatter
template <typename _T, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_set<_T, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const unordered_set<_T, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "{";
        auto __it = __v.begin();
        while (__it != __v.end()) {
            _Rst += _STD format("{}", *__it);
            ++__it;
            if (__it != __v.end()) {
                _Rst += ", ";
            }
        }
        _Rst.push_back('}');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for unordered_maps formatter
template <typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const unordered_map<_Key, _Tp, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "{";
        auto __it = __v.begin();
        while (__it != __v.end()) {
            _Rst += _STD format("({} -> {})", __it->first, __it->second);
            ++__it;
            if (__it != __v.end()) {
                _Rst += ", ";
            }
        }
        _Rst.push_back('}');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for unordered_multisets formatter
template <typename _T, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_multiset<_T, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const unordered_multiset<_T, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "{";
        auto __it = __v.begin();
        while (__it != __v.end()) {
            _Rst += _STD format("{}", *__it);
            ++__it;
            if (__it != __v.end()) {
                _Rst += ", ";
            }
        }
        _Rst.push_back('}');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

// Template specialization for unordered_multimaps formatter
template <typename _Key, typename _Tp, typename _Hash, typename _Pred, typename _Alloc>
struct formatter<unordered_multimap<_Key, _Tp, _Hash, _Pred, _Alloc>> : formatter<string_view> {
    template <typename _Fmt_Cont>
    auto format(const unordered_multimap<_Key, _Tp, _Hash, _Pred, _Alloc>& __v, _Fmt_Cont& __ctx) {
        string _Rst = "{";
        auto __it = __v.begin();
        while (__it != __v.end()) {
            _Rst += _STD format("({} -> {})", __it->first, __it->second);
            ++__it;
            if (__it != __v.end()) {
                _Rst += ", ";
            }
        }
        _Rst.push_back('}');
        return formatter<string_view>::format(_Rst, __ctx);
    }
};

_STD_END
#endif // _HAS_CXX20

#ifdef _HAS_CXX20
#define _CHAIN_UTILITY_BEGIN namespace clib::chain_utility {
#define _CHAIN_UTILITY_END   }

_CHAIN_UTILITY_BEGIN

_EXPORT_CLIB template <class _Cont, class ..._Args> // Immutable Range Operations
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&>&& _ARG_S_)
_NODISCARD _Cont operator|(_Cont _Val, _Args&& ..._Func) {
    for (auto&& _Ele : _Val) {
        (_Func(_Ele), _ARG_S_);
    }
    return _Val;
}

_EXPORT_CLIB template <class _Cont, class ..._Args> // Mutable Range Operations
    requires (_STD regular_invocable<_Args, typename _Cont::value_type&>&& _ARG_S_)
_NODISCARD _Cont& operator/(_Cont& _Val, _Args&& ..._Func) {
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
