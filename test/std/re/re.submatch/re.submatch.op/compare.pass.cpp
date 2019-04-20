//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class BidirectionalIterator> class sub_match;

/// @todo Update this section to the new proposed wording.

// template <class BiIter>
//     bool
//     operator==(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator!=(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator<(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator<=(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator>=(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator>(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator==(const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator!=(const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator<(const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& lhs,
//               const sub_match<BiIter>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator>(const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& lhs,
//               const sub_match<BiIter>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool operator>=(const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& lhs,
//                     const sub_match<BiIter>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator<=(const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator==(const sub_match<BiIter>& lhs,
//                const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator!=(const sub_match<BiIter>& lhs,
//                const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator<(const sub_match<BiIter>& lhs,
//               const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool operator>(const sub_match<BiIter>& lhs,
//                    const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator>=(const sub_match<BiIter>& lhs,
//                const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& rhs);
//
// template <class BiIter, class ST, class SA>
//     bool
//     operator<=(const sub_match<BiIter>& lhs,
//                const basic_string<typename iterator_traits<BiIter>::value_type, ST, SA>& rhs);
//
// template <class BiIter>
//     bool
//     operator==(typename iterator_traits<BiIter>::value_type const* lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator!=(typename iterator_traits<BiIter>::value_type const* lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator<(typename iterator_traits<BiIter>::value_type const* lhs,
//               const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator>(typename iterator_traits<BiIter>::value_type const* lhs,
//               const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator>=(typename iterator_traits<BiIter>::value_type const* lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator<=(typename iterator_traits<BiIter>::value_type const* lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator==(const sub_match<BiIter>& lhs,
//                typename iterator_traits<BiIter>::value_type const* rhs);
//
// template <class BiIter>
//     bool
//     operator!=(const sub_match<BiIter>& lhs,
//                typename iterator_traits<BiIter>::value_type const* rhs);
//
// template <class BiIter>
//     bool
//     operator<(const sub_match<BiIter>& lhs,
//               typename iterator_traits<BiIter>::value_type const* rhs);
//
// template <class BiIter>
//     bool
//     operator>(const sub_match<BiIter>& lhs,
//               typename iterator_traits<BiIter>::value_type const* rhs);
//
// template <class BiIter>
//     bool
//     operator>=(const sub_match<BiIter>& lhs,
//                typename iterator_traits<BiIter>::value_type const* rhs);
//
// template <class BiIter>
//     bool
//     operator<=(const sub_match<BiIter>& lhs,
//                typename iterator_traits<BiIter>::value_type const* rhs);
//
// template <class BiIter>
//     bool
//     operator==(typename iterator_traits<BiIter>::value_type const& lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator!=(typename iterator_traits<BiIter>::value_type const& lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator<(typename iterator_traits<BiIter>::value_type const& lhs,
//               const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator>(typename iterator_traits<BiIter>::value_type const& lhs,
//               const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator>=(typename iterator_traits<BiIter>::value_type const& lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator<=(typename iterator_traits<BiIter>::value_type const& lhs,
//                const sub_match<BiIter>& rhs);
//
// template <class BiIter>
//     bool
//     operator==(const sub_match<BiIter>& lhs,
//                typename iterator_traits<BiIter>::value_type const& rhs);
//
// template <class BiIter>
//     bool
//     operator!=(const sub_match<BiIter>& lhs,
//                typename iterator_traits<BiIter>::value_type const& rhs);
//
// template <class BiIter>
//     bool
//     operator<(const sub_match<BiIter>& lhs,
//               typename iterator_traits<BiIter>::value_type const& rhs);
//
// template <class BiIter>
//     bool
//     operator>(const sub_match<BiIter>& lhs,
//               typename iterator_traits<BiIter>::value_type const& rhs);
//
// template <class BiIter>
//     bool
//     operator>=(const sub_match<BiIter>& lhs,
//                typename iterator_traits<BiIter>::value_type const& rhs);
//
// template <class BiIter>
//     bool
//     operator<=(const sub_match<BiIter>& lhs,
//                typename iterator_traits<BiIter>::value_type const& rhs);

#include <regex>
#include <cassert>
#include "test_macros.h"

template <class CharT>
void
test(const std::basic_string<CharT>& x, const std::basic_string<CharT>& y, bool doCStrTests = true)
{
    typedef std::basic_string<CharT> string;
    typedef std::sub_match<typename string::const_iterator> sub_match;
    sub_match sm1;
    sm1.first = x.begin();
    sm1.second = x.end();
    sm1.matched = true;
    sub_match sm2;
    sm2.first = y.begin();
    sm2.second = y.end();
    sm2.matched = true;
    assert((sm1 == sm2) == (x == y));
    assert((sm1 != sm2) == (x != y));
    assert((sm1 < sm2) == (x < y));
    assert((sm1 > sm2) == (x > y));
    assert((sm1 <= sm2) == (x <= y));
    assert((sm1 >= sm2) == (x >= y));
    assert((x == sm2) == (x == y));
    assert((x != sm2) == (x != y));
    assert((x < sm2) == (x < y));
    assert((x > sm2) == (x > y));
    assert((x <= sm2) == (x <= y));
    assert((x >= sm2) == (x >= y));
    assert((sm1 == y) == (x == y));
    assert((sm1 != y) == (x != y));
    assert((sm1 < y) == (x < y));
    assert((sm1 > y) == (x > y));
    assert((sm1 <= y) == (x <= y));
    assert((sm1 >= y) == (x >= y));
    if (doCStrTests) {
        assert((x.c_str() == sm2) == (x == y));
        assert((x.c_str() != sm2) == (x != y));
        assert((x.c_str() < sm2) == (x < y));
        assert((x.c_str() > sm2) == (x > y));
        assert((x.c_str() <= sm2) == (x <= y));
        assert((x.c_str() >= sm2) == (x >= y));
        assert((sm1 == y.c_str()) == (x == y));
        assert((sm1 != y.c_str()) == (x != y));
        assert((sm1 < y.c_str()) == (x < y));
        assert((sm1 > y.c_str()) == (x > y));
        assert((sm1 <= y.c_str()) == (x <= y));
        assert((sm1 >= y.c_str()) == (x >= y));
        }
    assert((x[0] == sm2) == (string(1, x[0]) == y));
    assert((x[0] != sm2) == (string(1, x[0]) != y));
    assert((x[0] < sm2) == (string(1, x[0]) < y));
    assert((x[0] > sm2) == (string(1, x[0]) > y));
    assert((x[0] <= sm2) == (string(1, x[0]) <= y));
    assert((x[0] >= sm2) == (string(1, x[0]) >= y));
    assert((sm1 == y[0]) == (x == string(1, y[0])));
    assert((sm1 != y[0]) == (x != string(1, y[0])));
    assert((sm1 < y[0]) == (x < string(1, y[0])));
    assert((sm1 > y[0]) == (x > string(1, y[0])));
    assert((sm1 <= y[0]) == (x <= string(1, y[0])));
    assert((sm1 >= y[0]) == (x >= string(1, y[0])));
}

#if defined(__cpp_lib_string_view_regex)
template <class CharT>
void
test(std::basic_string_view<CharT> x, std::basic_string_view<CharT> y)
{
    typedef std::basic_string_view<CharT> string_view;
    typedef std::sub_match<typename string_view::const_iterator> sub_match;
    sub_match sm1;
    sm1.first = x.begin();
    sm1.second = x.end();
    sm1.matched = true;
    sub_match sm2;
    sm2.first = y.begin();
    sm2.second = y.end();
    sm2.matched = true;
    assert((sm1 == sm2) == (x == y));
    assert((sm1 != sm2) == (x != y));
    assert((sm1 < sm2) == (x < y));
    assert((sm1 > sm2) == (x > y));
    assert((sm1 <= sm2) == (x <= y));
    assert((sm1 >= sm2) == (x >= y));
    assert((x == sm2) == (x == y)); //
    assert((x != sm2) == (x != y));
    assert((x < sm2) == (x < y));
    assert((x > sm2) == (x > y));
    assert((x <= sm2) == (x <= y));
    assert((x >= sm2) == (x >= y));
    assert((sm1 == y) == (x == y));
    assert((sm1 != y) == (x != y));
    assert((sm1 < y) == (x < y));
    assert((sm1 > y) == (x > y));
    assert((sm1 <= y) == (x <= y));
    assert((sm1 >= y) == (x >= y));
    assert((x[0] == sm2) == (string_view(&x[0], 1) == y));
    assert((x[0] != sm2) == (string_view(&x[0], 1) != y));
    assert((x[0] < sm2) == (string_view(&x[0], 1) < y));
    assert((x[0] > sm2) == (string_view(&x[0], 1) > y));
    assert((x[0] <= sm2) == (string_view(&x[0], 1) <= y));
    assert((x[0] >= sm2) == (string_view(&x[0], 1) >= y));
    assert((sm1 == y[0]) == (x == string_view(&y[0], 1)));
    assert((sm1 != y[0]) == (x != string_view(&y[0], 1)));
    assert((sm1 < y[0]) == (x < string_view(&y[0], 1)));
    assert((sm1 > y[0]) == (x > string_view(&y[0], 1)));
    assert((sm1 <= y[0]) == (x <= string_view(&y[0], 1)));
    assert((sm1 >= y[0]) == (x >= string_view(&y[0], 1)));
}
#endif

int main(int, char**)
{
    test(std::string("123"), std::string("123"));
    test(std::string("1234"), std::string("123"));
    test(std::wstring(L"123"), std::wstring(L"123"));
    test(std::wstring(L"1234"), std::wstring(L"123"));
    test(std::string("123\000" "56", 6), std::string("123\000" "56", 6), false);
    test(std::wstring(L"123\000" L"56", 6), std::wstring(L"123\000" L"56", 6), false);

#if defined(__cpp_lib_string_view_regex)
    test(std::string_view("123"), std::string_view("123"));
    test(std::string_view("1234"), std::string_view("123"));
    test(std::wstring_view(L"123"), std::wstring_view(L"123"));
    test(std::wstring_view(L"1234"), std::wstring_view(L"123"));
    test(std::string_view("123\000" "56", 6), std::string_view("123\000" "56", 6));
    test(std::wstring_view(L"123\000" L"56", 6), std::wstring_view(L"123\000" L"56", 6));
#endif

  return 0;
}
