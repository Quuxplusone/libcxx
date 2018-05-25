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

#include "count_new.h"

int main(int, char**)
{
    globalMemCounter.reset();
    char buffer[100];
    std::experimental::pmr::monotonic_buffer_resource mono1((void *)buffer, sizeof buffer, std::experimental::pmr::new_delete_resource());
    std::experimental::pmr::memory_resource & r1 = mono1;

    // Check that construction with a buffer does not allocate anything from the upstream
    assert(globalMemCounter.checkNewCalledEq(0));

    // Check that an allocation that fits in the buffer does not allocate anything from the upstream
    void *ret = r1.allocate(50);
    assert(ret);
    assert(globalMemCounter.checkNewCalledEq(0));

    // Check a second allocation
    ret = r1.allocate(20);
    assert(ret);
    assert(globalMemCounter.checkNewCalledEq(0));

    r1.deallocate(ret, 50);
    assert(globalMemCounter.checkDeleteCalledEq(0));

    // Check an allocation that doesn't fit in the original buffer
    ret = r1.allocate(50);
    assert(ret);
    assert(globalMemCounter.checkNewCalledEq(1));

    r1.deallocate(ret, 50);
    assert(globalMemCounter.checkDeleteCalledEq(0));

    mono1.release();
    assert(globalMemCounter.checkDeleteCalledEq(1));
    assert(globalMemCounter.checkOutstandingNewEq(0));

  return 0;
}
