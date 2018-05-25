//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// REQUIRES: c++experimental
// UNSUPPORTED: c++98, c++03

// <experimental/memory_resource>

// class monotonic_buffer_resource

#include <experimental/memory_resource>
#include <cassert>

#include "count_new.hpp"

int main()
{
    globalMemCounter.reset();
    std::experimental::pmr::monotonic_buffer_resource mono1(1024, std::experimental::pmr::new_delete_resource());
    std::experimental::pmr::memory_resource & r1 = mono1;

    constexpr size_t big_alignment = 8 * alignof(std::max_align_t);
    static_assert(big_alignment > 4);

    void *ret = r1.allocate(2048, big_alignment);
    assert(ret != nullptr);
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.last_new_size >= 2048);
    // assert(globalMemCounter.last_new_align >= big_alignment);

    // Check that a single highly aligned allocation request doesn't
    // permanently "poison" the resource to allocate only super-aligned
    // blocks of memory.
    ret = r1.allocate(globalMemCounter.last_new_size, 4);
    assert(ret != nullptr);
    assert(globalMemCounter.checkNewCalledEq(2));
    // assert(globalMemCounter.last_new_align >= 4);
    // assert(globalMemCounter.last_new_align < big_alignment);
}
