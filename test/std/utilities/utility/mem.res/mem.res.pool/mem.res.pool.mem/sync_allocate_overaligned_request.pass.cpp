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

// class synchronized_pool_resource

#include <memory_resource>
#include <cassert>

#include "count_new.hpp"

static bool is_aligned_to(void *p, size_t alignment)
{
    void *p2 = p;
    size_t space = 1;
    void *result = std::align(alignment, 1, p2, space);
    return (result == p);
}

int main()
{
    globalMemCounter.reset();
    std::pmr::pool_options opts { 1, 1024 };
    std::pmr::synchronized_pool_resource sync1(opts, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = sync1;

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
}
