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
    {
        alignas(4) char buffer[17];
        std::experimental::pmr::monotonic_buffer_resource mono1(buffer + 1, 16, std::experimental::pmr::new_delete_resource());
        std::experimental::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret == buffer + 1);
        mono1.release();

        ret = r1.allocate(1, 2);
        assert(ret == buffer + 2);
        mono1.release();

        ret = r1.allocate(1, 4);
        assert(ret == buffer + 4);
        mono1.release();

        // Test a size that is just big enough to fit in the buffer,
        // but can't fit if it's aligned.
        ret = r1.allocate(16, 1);
        assert(ret == buffer + 1);
        mono1.release();

        assert(globalMemCounter.checkNewCalledEq(0));
        ret = r1.allocate(16, 2);
        assert(globalMemCounter.checkNewCalledEq(1));
        assert(globalMemCounter.last_new_size >= 16);
        // assert(globalMemCounter.last_new_align >= 2);
        mono1.release();
        assert(globalMemCounter.checkDeleteCalledEq(1));
        // assert(globalMemCounter.last_new_align == globalMemCounter.last_delete_align);
    }

  return 0;
}
