//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <deque>

// namespace std::pmr {
// template <class T>
// using deque =
//     ::std::deque<T, polymorphic_allocator<T>>
//
// } // namespace std::pmr

#include <deque>
#include <memory_resource>
#include <type_traits>
#include <cassert>

int main()
{
    using StdDeque = std::deque<int, std::pmr::polymorphic_allocator<int>>;
    using PmrDeque = std::pmr::deque<int>;
    static_assert(std::is_same<StdDeque, PmrDeque>::value, "");
    PmrDeque d;
    assert(d.get_allocator().resource() == std::pmr::get_default_resource());
}
