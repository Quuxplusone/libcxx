//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <unordered_map>
// UNSUPPORTED: c++98, c++03, c++11, c++14
// UNSUPPORTED: libcpp-no-deduction-guides

//  template<class InputIterator,
//           class Hash = hash<iter-key-type<InputIterator>>,
//           class Pred = equal_to<iter-key-type<InputIterator>>,
//           class Allocator = allocator<iter-to-alloc-type<InputIterator>>>
//  unordered_map(InputIterator, InputIterator, typename see below::size_type = see below,
//                Hash = Hash(), Pred = Pred(), Allocator = Allocator())
//    -> unordered_map<iter-key-type<InputIterator>, iter-mapped-type<InputIterator>, Hash, Pred,
//                     Allocator>;
//
//  template<class Key, class T, class Hash = hash<Key>,
//           class Pred = equal_to<Key>, class Allocator = allocator<pair<const Key, T>>>
//  unordered_map(initializer_list<pair<Key, T>>,
//                typename see below::size_type = see below, Hash = Hash(),
//                Pred = Pred(), Allocator = Allocator())
//    -> unordered_map<Key, T, Hash, Pred, Allocator>;
//
//  template<class InputIterator, class Allocator>
//  unordered_map(InputIterator, InputIterator, typename see below::size_type, Allocator)
//    -> unordered_map<iter-key-type<InputIterator>, iter-mapped-type<InputIterator>,
//                     hash<iter-key-type<InputIterator>>,
//                     equal_to<iter-key-type<InputIterator>>, Allocator>;
//
//  template<class InputIterator, class Allocator>
//  unordered_map(InputIterator, InputIterator, Allocator)
//    -> unordered_map<iter-key-type<InputIterator>, iter-mapped-type<InputIterator>,
//                     hash<iter-key-type<InputIterator>>,
//                     equal_to<iter-key-type<InputIterator>>, Allocator>;
//
//  template<class InputIterator, class Hash, class Allocator>
//  unordered_map(InputIterator, InputIterator, typename see below::size_type, Hash, Allocator)
//    -> unordered_map<iter-key-type<InputIterator>, iter-mapped-type<InputIterator>, Hash,
//                     equal_to<iter-key-type<InputIterator>>, Allocator>;
//
//  template<class Key, class T, class Allocator>
//  unordered_map(initializer_list<pair<Key, T>>, typename see below::size_type, Allocator)
//    -> unordered_map<Key, T, hash<Key>, equal_to<Key>, Allocator>;
//
//  template<class Key, class T, class Allocator>
//  unordered_map(initializer_list<pair<Key, T>>, Allocator)
//    -> unordered_map<Key, T, hash<Key>, equal_to<Key>, Allocator>;
//
//  template<class Key, class T, class Hash, class Allocator>
//  unordered_map(initializer_list<pair<Key, T>>, typename see below::size_type, Hash,
//                Allocator)
//    -> unordered_map<Key, T, Hash, equal_to<Key>, Allocator>;

#include <algorithm> // is_permutation
#include <cassert>
#include <climits> // INT_MAX
#include <type_traits>
#include <unordered_map>

#include "test_allocator.h"

int main(int, char**)
{
    using P = std::pair<const int, long>;
    std::pair<const int, long> expected_m[] = { {1,1}, {2,2}, {3,1}, {INT_MAX,1} };

    {
    const P arr[] = { {1,1}, {2,2}, {1,1}, {INT_MAX,1}, {3,1} };
    std::unordered_map m(std::begin(arr), std::end(arr));
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    }

    {
    const P arr[] = { {1,1}, {2,2}, {1,1}, {INT_MAX,1}, {3,1} };
    std::unordered_map m(std::begin(arr), std::end(arr), 42);
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    }

    {
    const P arr[] = { {1,1}, {2,2}, {1,1}, {INT_MAX,1}, {3,1} };
    std::unordered_map m(std::begin(arr), std::end(arr), 42, std::hash<short>());
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<short>, std::equal_to<int>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    }

    {
    const P arr[] = { {1,1}, {2,2}, {1,1}, {INT_MAX,1}, {3,1} };
    std::unordered_map m(std::begin(arr), std::end(arr), 42, std::hash<short>(), std::equal_to<>());
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<short>, std::equal_to<>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    }

    {
    const P arr[] = { {1,1}, {2,2}, {1,1}, {INT_MAX,1}, {3,1} };
    std::unordered_map m(std::begin(arr), std::end(arr), 42, std::hash<short>(), std::equal_to<>(), test_allocator<P>(0, 41));
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<short>, std::equal_to<>, test_allocator<P>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    assert(m.get_allocator().get_id() == 41);
    }

    {
    std::unordered_map m { P{1,1L}, P{2,2L}, P{1,1L}, P{INT_MAX,1L}, P{3,1L} };
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    }

    {
    std::unordered_map m({ P{1,1L}, P{2,2L}, P{1,1L}, P{INT_MAX,1L}, P{3,1L} }, 42);
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    }

    {
    std::unordered_map m({ P{1,1L}, P{2,2L}, P{1,1L}, P{INT_MAX,1L}, P{3,1L} }, 42, std::hash<short>());
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<short>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    }

    {
    std::unordered_map m({ P{1,1L}, P{2,2L}, P{1,1L}, P{INT_MAX,1L}, P{3,1L} }, 42, std::hash<short>(), std::equal_to<>());
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<short>, std::equal_to<>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    }

    {
    std::unordered_map m({ P{1,1L}, P{2,2L}, P{1,1L}, P{INT_MAX,1L}, P{3,1L} }, 42, std::hash<short>(), std::equal_to<>(), test_allocator<P>(0, 44));
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<short>, std::equal_to<>, test_allocator<P>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    assert(m.get_allocator().get_id() == 44);
    }

    {
    const P arr[] = { {1,1}, {2,2}, {1,1}, {INT_MAX,1}, {3,1} };
    std::unordered_map m(std::begin(arr), std::end(arr), 42, test_allocator<P>(0, 45));
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<int>, std::equal_to<int>, test_allocator<P>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    assert(m.get_allocator().get_id() == 45);
    }

    {
    const P arr[] = { {1,1}, {2,2}, {1,1}, {INT_MAX,1}, {3,1} };
    std::unordered_map m(std::begin(arr), std::end(arr), 42, std::hash<short>(), test_allocator<P>(0, 46));
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<short>, std::equal_to<int>, test_allocator<P>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    assert(m.get_allocator().get_id() == 46);
    }

    {
    std::unordered_map m({ P{1,1L}, P{2,2L}, P{1,1L}, P{INT_MAX,1L}, P{3,1L} }, 42, test_allocator<P>(0, 47));
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<int>, std::equal_to<int>, test_allocator<P>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    assert(m.get_allocator().get_id() == 47);
    }

    {
    std::unordered_map m({ P{1,1L}, P{2,2L}, P{1,1L}, P{INT_MAX,1L}, P{3,1L} }, 42, std::hash<short>(), test_allocator<P>(0, 48));
    ASSERT_SAME_TYPE(decltype(m), std::unordered_map<int, long, std::hash<short>, std::equal_to<int>, test_allocator<P>>);
    assert(std::is_permutation(m.begin(), m.end(), std::begin(expected_m), std::end(expected_m)));
    assert(m.get_allocator().get_id() == 48);
    }

  return 0;
}
