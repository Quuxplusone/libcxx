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

// class synchronized_pool_resource

#include <experimental/memory_resource>
#include <cassert>

#include "count_new.h"

int main(int, char**)
{
    {
        globalMemCounter.reset();

        std::experimental::pmr::synchronized_pool_resource sync1(std::experimental::pmr::new_delete_resource());
        std::experimental::pmr::memory_resource & r1 = sync1;

        void *ret = r1.allocate(50);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        r1.deallocate(ret, 50);
        sync1.release();
        assert(globalMemCounter.checkDeleteCalledGreaterThan(0));
        assert(globalMemCounter.checkOutstandingNewEq(0));

        globalMemCounter.reset();

        ret = r1.allocate(500);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        // Check that the destructor calls release()
    }
    assert(globalMemCounter.checkDeleteCalledGreaterThan(0));
    assert(globalMemCounter.checkOutstandingNewEq(0));

    return 0;
}
