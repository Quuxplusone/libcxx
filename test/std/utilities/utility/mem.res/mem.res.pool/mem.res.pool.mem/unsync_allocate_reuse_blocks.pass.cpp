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

// class unsynchronized_pool_resource

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
    std::pmr::pool_options opts { 1, 256 };
    std::pmr::unsynchronized_pool_resource unsync1(opts, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = unsync1;

    void *ret = r1.allocate(8);
    assert(ret != nullptr);
    assert(is_aligned_to(ret, 8));
    assert(globalMemCounter.checkNewCalledGreaterThan(0));
    int new_called = globalMemCounter.new_called;

    // After deallocation, the pool for 8-byte blocks should have at least one vacancy.
    r1.deallocate(ret, 8);
    assert(globalMemCounter.new_called == new_called);
    assert(globalMemCounter.checkDeleteCalledEq(0));

    // This should return an existing block from the pool: no new allocations.
    ret = r1.allocate(8);
    assert(ret != nullptr);
    assert(is_aligned_to(ret, 8));
    assert(globalMemCounter.new_called == new_called);
    assert(globalMemCounter.checkDeleteCalledEq(0));
}
