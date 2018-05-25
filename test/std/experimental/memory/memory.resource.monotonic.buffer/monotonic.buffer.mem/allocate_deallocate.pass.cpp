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
    {
        globalMemCounter.reset();

        std::experimental::pmr::monotonic_buffer_resource mono1(std::experimental::pmr::new_delete_resource());
        std::experimental::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(50);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        // mem.res.monotonic.buffer 1.2
        // A call to deallocate has no effect, thus the amount of memory
        // consumed increases monotonically until the resource is destroyed.
        // Check that deallocate is a no-op
        r1.deallocate(ret, 50);
        assert(globalMemCounter.checkDeleteCalledEq(0));

        mono1.release();
        assert(globalMemCounter.checkDeleteCalledEq(1));
        assert(globalMemCounter.checkOutstandingNewEq(0));

        globalMemCounter.reset();

        ret = r1.allocate(500);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        // Check that the destructor calls release()
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));
    assert(globalMemCounter.checkOutstandingNewEq(0));

  return 0;
}
