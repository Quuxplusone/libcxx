// -*- C++ -*-
//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <regex>

// template <class charT>
// struct regex_traits
// {
// public:
//     typedef charT                        char_type;
//     typedef basic_string<char_type>      string_type;
//     typedef basic_string_view<char_type> string_view_type;
//     typedef locale                       locale_type;

#include <regex>
#include <type_traits>
#include "test_macros.h"

int main(int, char**)
{
    static_assert((std::is_same<std::regex_traits<char>::char_type, char>::value), "");
    static_assert((std::is_same<std::regex_traits<char>::string_type, std::string>::value), "");
#if defined(__cpp_lib_string_view_regex)
    static_assert((std::is_same<std::regex_traits<char>::string_view_type, std::string_view>::value), "");
#endif
    static_assert((std::is_same<std::regex_traits<char>::locale_type, std::locale>::value), "");
    static_assert((std::is_same<std::regex_traits<wchar_t>::char_type, wchar_t>::value), "");
    static_assert((std::is_same<std::regex_traits<wchar_t>::string_type, std::wstring>::value), "");
#if defined(__cpp_lib_string_view_regex)
    static_assert((std::is_same<std::regex_traits<wchar_t>::string_view_type, std::wstring_view>::value), "");
#endif
    static_assert((std::is_same<std::regex_traits<wchar_t>::locale_type, std::locale>::value), "");

  return 0;
}
