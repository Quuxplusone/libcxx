//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17

// <regex>

// template <class charT, class traits = regex_traits<charT>> class basic_regex;

// template <class ST>
//    basic_regex(basic_string_view<charT, ST> s);

#include <regex>
#include <cassert>
#include "test_macros.h"

template <class String_view>
void
test(const String_view& p, unsigned mc)
{
    std::basic_regex<typename String_view::value_type> r(p);
    assert(r.flags() == std::regex_constants::ECMAScript);
    assert(r.mark_count() == mc);
}

int main(int, char**)
{
    test(std::string_view("\\(a\\)"), 0);
    test(std::string_view("\\(a[bc]\\)"), 0);
    test(std::string_view("\\(a\\([bc]\\)\\)"), 0);
    test(std::string_view("(a([bc]))"), 2);

  return 0;
}
