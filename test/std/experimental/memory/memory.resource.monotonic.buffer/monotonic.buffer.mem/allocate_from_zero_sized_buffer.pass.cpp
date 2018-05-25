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
        char buffer[100];
        std::experimental::pmr::monotonic_buffer_resource mono1((void *)buffer, 0, std::experimental::pmr::new_delete_resource());
        std::experimental::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret != nullptr);
        assert(globalMemCounter.checkNewCalledEq(1));
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));

    globalMemCounter.reset();
    {
        std::experimental::pmr::monotonic_buffer_resource mono1(nullptr, 0, std::experimental::pmr::new_delete_resource());
        std::experimental::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret != nullptr);
        assert(globalMemCounter.checkNewCalledEq(1));
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));

  return 0;
}
