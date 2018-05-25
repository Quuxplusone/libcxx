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

// class unsynchronized_pool_resource

#include <experimental/memory_resource>
#include <cassert>

#include "count_new.hpp"

bool is_aligned_to(void *p, size_t alignment)
{
    void *p2 = p;
    size_t space = 1;
    void *result = std::align(alignment, 1, p2, space);
    return (result == p);
}

int main(int, char**)
{
#ifndef TEST_HAS_NO_ALIGNED_ALLOCATION

    globalMemCounter.reset();
    std::experimental::pmr::pool_options opts { 1, 1024 };
    std::experimental::pmr::unsynchronized_pool_resource unsync1(opts, std::experimental::pmr::new_delete_resource());
    std::experimental::pmr::memory_resource & r1 = unsync1;

    constexpr size_t big_alignment = 8 * alignof(std::max_align_t);
    static_assert(big_alignment > 4);

    assert(globalMemCounter.checkNewCalledEq(0));

    void *ret = r1.allocate(2048, big_alignment);
    assert(ret != nullptr);
    assert(is_aligned_to(ret, big_alignment));
    assert(globalMemCounter.checkNewCalledGreaterThan(0));

    ret = r1.allocate(16, 4);
    assert(ret != nullptr);
    assert(is_aligned_to(ret, 4));
    assert(globalMemCounter.checkNewCalledGreaterThan(1));

#endif

    return 0;
}
