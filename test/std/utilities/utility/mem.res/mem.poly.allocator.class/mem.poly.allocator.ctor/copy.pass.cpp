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

// polymorphic_allocator<T>::polymorphic_allocator(polymorphic_allocator const &);

#include <memory_resource>
#include <type_traits>
#include <cassert>

int main(int, char**)
{
    typedef std::pmr::polymorphic_allocator<void> A1;
    {
        static_assert(
            std::is_copy_constructible<A1>::value, ""
          );
        static_assert(
            std::is_move_constructible<A1>::value, ""
          );
    }
    // copy
    {
        A1 const a((std::pmr::memory_resource*)42);
        A1 const a2(a);
        assert(a.resource() == a2.resource());
    }
    // move
    {
        A1 a((std::pmr::memory_resource*)42);
        A1 a2(std::move(a));
        assert(a.resource() == a2.resource());
        assert(a2.resource() == (std::pmr::memory_resource*)42);
    }

  return 0;
}
