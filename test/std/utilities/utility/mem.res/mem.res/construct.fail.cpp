//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <memory_resource>

// Check that memory_resource is not constructible

#include <memory_resource>
#include <type_traits>
#include <cassert>

namespace ex = std::pmr;

int main()
{
    ex::memory_resource m; // expected-error {{variable type 'ex::memory_resource' is an abstract class}}
}
