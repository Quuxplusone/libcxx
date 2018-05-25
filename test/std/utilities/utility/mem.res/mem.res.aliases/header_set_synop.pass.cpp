//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <set>

// namespace std::pmr {
// template <class V, class Compare = less<V> >
// using set =
//     ::std::set<V, Compare, polymorphic_allocator<V>>
//
// template <class V, class Compare = less<V> >
// using multiset =
//     ::std::multiset<V, Compare, polymorphic_allocator<V>>
//
// } // namespace std::pmr

#include <set>
#include <memory_resource>
#include <type_traits>
#include <cassert>

int main()
{
    using V = char;
    using DC = std::less<V>;
    using OC = std::greater<V>;
    {
        using StdSet = std::set<V, DC, std::pmr::polymorphic_allocator<V>>;
        using PmrSet = std::pmr::set<V>;
        static_assert(std::is_same<StdSet, PmrSet>::value, "");
    }
    {
        using StdSet = std::set<V, OC, std::pmr::polymorphic_allocator<V>>;
        using PmrSet = std::pmr::set<V, OC>;
        static_assert(std::is_same<StdSet, PmrSet>::value, "");
    }
    {
        std::pmr::set<int> m;
        assert(m.get_allocator().resource() == std::pmr::get_default_resource());
    }
    {
        using StdSet = std::multiset<V, DC, std::pmr::polymorphic_allocator<V>>;
        using PmrSet = std::pmr::multiset<V>;
        static_assert(std::is_same<StdSet, PmrSet>::value, "");
    }
    {
        using StdSet = std::multiset<V, OC, std::pmr::polymorphic_allocator<V>>;
        using PmrSet = std::pmr::multiset<V, OC>;
        static_assert(std::is_same<StdSet, PmrSet>::value, "");
    }
    {
        std::pmr::multiset<int> m;
        assert(m.get_allocator().resource() == std::pmr::get_default_resource());
    }
}
