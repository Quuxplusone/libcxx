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

// class monotonic_buffer_resource

#include <memory_resource>
#include <cassert>

#include "count_new.hpp"

int main()
{
    globalMemCounter.reset();
    {
        char buffer[100];
        std::pmr::monotonic_buffer_resource mono1((void *)buffer, 0, std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret != nullptr);
        assert(globalMemCounter.checkNewCalledEq(1));
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));

    globalMemCounter.reset();
    {
        std::pmr::monotonic_buffer_resource mono1(nullptr, 0, std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret != nullptr);
        assert(globalMemCounter.checkNewCalledEq(1));
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));
}
