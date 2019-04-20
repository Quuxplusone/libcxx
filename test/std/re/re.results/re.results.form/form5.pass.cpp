//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17

// <regex>

// class match_results<BidirectionalIterator, Allocator>

// template <class OutputIter, class ST>
//   OutputIter
//   format(OutputIter out, basic_string_view<char_type, ST> fmt,
//          regex_constants::match_flag_type flags = regex_constants::format_default) const;

#include <iostream>

#include <regex>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "test_allocator.h"

int main(int, char**)
{
    typedef std::basic_string_view<char, std::char_traits<char> > nstr;
    typedef std::basic_string_view<wchar_t, std::char_traits<wchar_t> > wstr;
    {
        std::match_results<const char*> m;
        const char s[] = "abcdefghijk";
        assert(std::regex_search(s, m, std::regex("cd((e)fg)hi")));

        char out[100] = {0};
        nstr fmt("prefix: $`, match: $&, suffix: $', m[1]: $1, m[2]: $2");
        char* r = m.format(output_iterator<char*>(out), fmt).base();
        assert(r == out + 58);
        assert(std::string_view(out) == "prefix: ab, match: cdefghi, suffix: jk, m[1]: efg, m[2]: e");
    }
    {
        std::match_results<const char*> m;
        const char s[] = "abcdefghijk";
        assert(std::regex_search(s, m, std::regex("cd((e)fg)hi")));

        char out[100] = {0};
        nstr fmt("prefix: $`, match: $&, suffix: $', m[1]: $1, m[2]: $2");
        char* r = m.format(output_iterator<char*>(out),
                    fmt, std::regex_constants::format_sed).base();
        assert(r == out + 59);
        assert(std::string_view(out) == "prefix: $`, match: $cdefghi, suffix: $', m[1]: $1, m[2]: $2");
    }
    {
        std::match_results<const char*> m;
        const char s[] = "abcdefghijk";
        assert(std::regex_search(s, m, std::regex("cd((e)fg)hi")));

        char out[100] = {0};
        nstr fmt("match: &, m[1]: \\1, m[2]: \\2");
        char* r = m.format(output_iterator<char*>(out),
                    fmt, std::regex_constants::format_sed).base();
        assert(r == out + 34);
        assert(std::string_view(out) == "match: cdefghi, m[1]: efg, m[2]: e");
    }

    {
        std::match_results<const wchar_t*> m;
        const wchar_t s[] = L"abcdefghijk";
        assert(std::regex_search(s, m, std::wregex(L"cd((e)fg)hi")));

        wchar_t out[100] = {0};
        wstr fmt(L"prefix: $`, match: $&, suffix: $', m[1]: $1, m[2]: $2");
        wchar_t* r = m.format(output_iterator<wchar_t*>(out), fmt).base();
        assert(r == out + 58);
        assert(std::wstring_view(out) == L"prefix: ab, match: cdefghi, suffix: jk, m[1]: efg, m[2]: e");
    }
    {
        std::match_results<const wchar_t*> m;
        const wchar_t s[] = L"abcdefghijk";
        assert(std::regex_search(s, m, std::wregex(L"cd((e)fg)hi")));

        wchar_t out[100] = {0};
        wstr fmt(L"prefix: $`, match: $&, suffix: $', m[1]: $1, m[2]: $2");
        wchar_t* r = m.format(output_iterator<wchar_t*>(out),
                    fmt, std::regex_constants::format_sed).base();
        assert(r == out + 59);
        assert(std::wstring_view(out) == L"prefix: $`, match: $cdefghi, suffix: $', m[1]: $1, m[2]: $2");
    }
    {
        std::match_results<const wchar_t*> m;
        const wchar_t s[] = L"abcdefghijk";
        assert(std::regex_search(s, m, std::wregex(L"cd((e)fg)hi")));

        wchar_t out[100] = {0};
        wstr fmt(L"match: &, m[1]: \\1, m[2]: \\2");
        wchar_t* r = m.format(output_iterator<wchar_t*>(out),
                    fmt, std::regex_constants::format_sed).base();
        assert(r == out + 34);
        assert(std::wstring_view(out) == L"match: cdefghi, m[1]: efg, m[2]: e");
    }

  return 0;
}
