//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <string>

// namespace std::pmr {
// template <class Char, class Traits = ...>
// using basic_string =
//     ::std::basic_string<Char, Traits, polymorphic_allocator<Char>>
//
// typedef ... string
// typedef ... u16string
// typedef ... u32string
// typedef ... wstring
//
// } // namespace std::pmr

#include <string>
#include <memory_resource>
#include <type_traits>
#include <cassert>

#include "constexpr_char_traits.hpp"

namespace pmr = std::pmr;

template <class Char, class PmrTypedef>
void test_string_typedef() {
    using StdStr = std::basic_string<Char, std::char_traits<Char>,
                                     std::pmr::polymorphic_allocator<Char>>;
    using PmrStr = std::pmr::basic_string<Char>;
    static_assert(std::is_same<StdStr, PmrStr>::value, "");
    static_assert(std::is_same<PmrStr, PmrTypedef>::value, "");
}

template <class Char, class Traits>
void test_basic_string_alias() {
    using StdStr = std::basic_string<Char, Traits,
                                     std::pmr::polymorphic_allocator<Char>>;
    using PmrStr = std::pmr::basic_string<Char, Traits>;
    static_assert(std::is_same<StdStr, PmrStr>::value, "");
}

int main(int, char**)
{
    {
        test_string_typedef<char,     std::pmr::string>();
        test_string_typedef<wchar_t,  std::pmr::wstring>();
        test_string_typedef<char16_t, std::pmr::u16string>();
        test_string_typedef<char32_t, std::pmr::u32string>();
    }
    {
        test_basic_string_alias<char,    constexpr_char_traits<char>>();
        test_basic_string_alias<wchar_t, constexpr_char_traits<wchar_t>>();
        test_basic_string_alias<char16_t, constexpr_char_traits<char16_t>>();
        test_basic_string_alias<char32_t, constexpr_char_traits<char32_t>>();
    }
    {
        // Check that std::basic_string has been included and is complete.
        std::pmr::string s;
        assert(s.get_allocator().resource() == std::pmr::get_default_resource());
    }

  return 0;
}
