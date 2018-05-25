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

void test_geometric_progression()
{
    // mem.res.monotonic.buffer 1.3
    // Each additional buffer is larger than the previous one, following a
    // geometric progression.

    globalMemCounter.reset();
    std::pmr::monotonic_buffer_resource mono1(100, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = mono1;

    assert(globalMemCounter.checkNewCalledEq(0));
    size_t next_buffer_size = 100;
    void *ret = r1.allocate(10, 1);
    assert(ret != nullptr);
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.last_new_size >= next_buffer_size);
    next_buffer_size = globalMemCounter.last_new_size + 1;

    int new_called = 1;
    while (new_called < 5) {
        ret = r1.allocate(10, 1);
        if (globalMemCounter.new_called > new_called) {
            assert(globalMemCounter.new_called == new_called + 1);
            assert(globalMemCounter.last_new_size >= next_buffer_size);
            next_buffer_size = globalMemCounter.last_new_size + 1;
            new_called += 1;
        }
    }
}

int main()
{
    test_geometric_progression();
}
