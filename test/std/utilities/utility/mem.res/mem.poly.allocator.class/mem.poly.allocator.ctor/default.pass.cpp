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

// polymorphic_allocator<T>::polymorphic_allocator() noexcept

#include <memory_resource>
#include <type_traits>
#include <cassert>

#include "test_std_memory_resource.h"

namespace ex = std::pmr;

int main(int, char**)
{
    {
        static_assert(
            std::is_nothrow_default_constructible<ex::polymorphic_allocator<void>>::value
          , "Must me nothrow default constructible"
          );
    }
    {
        // test that the allocator gets its resource from get_default_resource
        TestResource R1(42);
        ex::set_default_resource(&R1);

        typedef ex::polymorphic_allocator<void> A;
        A const a;
        assert(a.resource() == &R1);

        ex::set_default_resource(nullptr);
        A const a2;
        assert(a.resource() == &R1);
        assert(a2.resource() == ex::new_delete_resource());
    }

  return 0;
}
