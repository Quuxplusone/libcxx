//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17

// <regex>

// template <class BidirectionalIterator> class sub_match;

// int compare(string_view_type s) const;

#include <regex>
#include <cassert>
#include "test_macros.h"

int main()
{
    {
        typedef char CharT;
        typedef std::sub_match<const CharT*> SM;
        typedef SM::string_view_type string_view;
        SM sm = SM();
        assert(sm.compare(string_view()) == 0);
        const CharT s[] = {'1', '2', '3', 0};
        sm.first = s;
        sm.second = s + 3;
        sm.matched = true;
        assert(sm.compare(string_view()) > 0);
        assert(sm.compare(string_view("123")) == 0);
    }
    {
        typedef wchar_t CharT;
        typedef std::sub_match<const CharT*> SM;
        typedef SM::string_view_type string_view;
        SM sm = SM();
        assert(sm.compare(string_view()) == 0);
        const CharT s[] = {'1', '2', '3', 0};
        sm.first = s;
        sm.second = s + 3;
        sm.matched = true;
        assert(sm.compare(string_view()) > 0);
        assert(sm.compare(string_view(L"123")) == 0);
    }
}
