//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// <any>
//
// class any

#include <any>

#if __has_extension(trivially_relocatable)

class _LIBCPP_TRIVIALLY_RELOCATABLE Reloc { ~Reloc(); };

static_assert(std::is_trivially_relocatable<int>::value, "");
static_assert(std::is_trivially_relocatable<Reloc>::value, "");

static_assert(std::is_trivially_relocatable< std::any >::value, "");

#endif

int main() {}
