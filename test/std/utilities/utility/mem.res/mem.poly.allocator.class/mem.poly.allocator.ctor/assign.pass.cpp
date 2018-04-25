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

// polymorphic_allocator operator=(polymorphic_allocator const &) = delete

#include <memory_resource>
#include <type_traits>
#include <cassert>

int main()
{
    typedef std::pmr::polymorphic_allocator<void> T;
    static_assert(!std::is_copy_assignable<T>::value, "");
    static_assert(!std::is_move_assignable<T>::value, "");
}
