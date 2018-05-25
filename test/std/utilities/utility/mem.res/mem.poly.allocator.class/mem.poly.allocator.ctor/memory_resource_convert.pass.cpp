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

// template <class T> class polymorphic_allocator

// polymorphic_allocator<T>::polymorphic_allocator(memory_resource *)

#include <memory_resource>
#include <type_traits>
#include <cassert>

#include "test_memory_resource.hpp"

namespace ex = std::pmr;

int main(int, char**)
{
    {
        typedef ex::polymorphic_allocator<void> A;
        static_assert(
            std::is_convertible<decltype(nullptr), A>::value
          , "Must be convertible"
          );
        static_assert(
            std::is_convertible<ex::memory_resource *, A>::value
          , "Must be convertible"
          );
    }
    {
        typedef ex::polymorphic_allocator<void> A;
        TestResource R;
        A const a(&R);
        assert(a.resource() == &R);
    }

  return 0;
}
