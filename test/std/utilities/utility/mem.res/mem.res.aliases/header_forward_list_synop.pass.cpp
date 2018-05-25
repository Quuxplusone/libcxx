//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <forward_list>

// namespace std::pmr {
// template <class T>
// using forward_list =
//     ::std::forward_list<T, polymorphic_allocator<T>>
//
// } // namespace std::pmr

#include <forward_list>
#include <memory_resource>
#include <type_traits>
#include <cassert>

int main()
{
    using StdForwardList = std::forward_list<int, std::pmr::polymorphic_allocator<int>>;
    using PmrForwardList = std::pmr::forward_list<int>;
    static_assert(std::is_same<StdForwardList, PmrForwardList>::value, "");
    PmrForwardList d;
    assert(d.get_allocator().resource() == std::pmr::get_default_resource());
}
