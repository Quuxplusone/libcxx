//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <list>

// namespace std::pmr {
// template <class T>
// using list =
//     ::std::list<T, polymorphic_allocator<T>>
//
// } // namespace std::pmr

#include <list>
#include <memory_resource>
#include <type_traits>
#include <cassert>

int main()
{
    using StdList = std::list<int, std::pmr::polymorphic_allocator<int>>;
    using PmrList = std::pmr::list<int>;
    static_assert(std::is_same<StdList, PmrList>::value, "");
    PmrList d;
    assert(d.get_allocator().resource() == std::pmr::get_default_resource());
}
