//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// REQUIRES: c++17

// <string>

// namespace std::pmr {
//
// typedef ... string
// typedef ... u16string
// typedef ... u32string
// typedef ... wstring
//
// } // namespace std::pmr

#include <string>

int main(int, char**)
{
    {
        // Check that std::pmr::string is usable without <memory_resource>.
        std::pmr::string s;
        std::pmr::wstring ws;
        std::pmr::u16string u16s;
        std::pmr::u32string u32s;
    }

  return 0;
}
