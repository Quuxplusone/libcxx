//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <map>

// namespace std::pmr {
// template <class K, class V, class Compare = less<Key> >
// using map =
//     ::std::map<K, V, Compare, polymorphic_allocator<pair<const K, V>>>
//
// template <class K, class V, class Compare = less<Key> >
// using multimap =
//     ::std::multimap<K, V, Compare, polymorphic_allocator<pair<const K, V>>>
//
// } // namespace std::pmr

#include <map>
#include <memory_resource>
#include <type_traits>
#include <cassert>

int main()
{
    using K = int;
    using V = char;
    using DC = std::less<int>;
    using OC = std::greater<int>;
    using P = std::pair<const K, V>;
    {
        using StdMap = std::map<K, V, DC, std::pmr::polymorphic_allocator<P>>;
        using PmrMap = std::pmr::map<K, V>;
        static_assert(std::is_same<StdMap, PmrMap>::value, "");
    }
    {
        using StdMap = std::map<K, V, OC, std::pmr::polymorphic_allocator<P>>;
        using PmrMap = std::pmr::map<K, V, OC>;
        static_assert(std::is_same<StdMap, PmrMap>::value, "");
    }
    {
        std::pmr::map<int, int> m;
        assert(m.get_allocator().resource() == std::pmr::get_default_resource());
    }
    {
        using StdMap = std::multimap<K, V, DC, std::pmr::polymorphic_allocator<P>>;
        using PmrMap = std::pmr::multimap<K, V>;
        static_assert(std::is_same<StdMap, PmrMap>::value, "");
    }
    {
        using StdMap = std::multimap<K, V, OC, std::pmr::polymorphic_allocator<P>>;
        using PmrMap = std::pmr::multimap<K, V, OC>;
        static_assert(std::is_same<StdMap, PmrMap>::value, "");
    }
    {
        std::pmr::multimap<int, int> m;
        assert(m.get_allocator().resource() == std::pmr::get_default_resource());
    }
}
