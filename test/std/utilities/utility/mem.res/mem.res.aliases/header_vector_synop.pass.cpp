//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <vector>

// namespace std::pmr {
// template <class T>
// using vector =
//     ::std::vector<T, polymorphic_allocator<T>>
//
// } // namespace std::pmr

#include <vector>
#include <memory_resource>
#include <type_traits>
#include <cassert>

int main()
{
    using StdVector = std::vector<int, std::pmr::polymorphic_allocator<int>>;
    using PmrVector = std::pmr::vector<int>;
    static_assert(std::is_same<StdVector, PmrVector>::value, "");
    PmrVector d;
    assert(d.get_allocator().resource() == std::pmr::get_default_resource());
}
