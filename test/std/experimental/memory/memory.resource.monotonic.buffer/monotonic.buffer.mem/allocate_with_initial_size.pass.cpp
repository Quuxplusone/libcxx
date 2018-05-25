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

#include "count_new.hpp"

void test(size_t initial_buffer_size)
{
    globalMemCounter.reset();

    std::experimental::pmr::monotonic_buffer_resource mono1(
        initial_buffer_size,
        std::experimental::pmr::new_delete_resource()
    );
    assert(globalMemCounter.checkNewCalledEq(0));

    mono1.allocate(1, 1);
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.last_new_size >= initial_buffer_size);

    mono1.allocate(initial_buffer_size - 1, 1);
    assert(globalMemCounter.checkNewCalledEq(1));
}

int main(int, char**)
{
    test(1);
    test(8);
    test(10);
    test(100);
    test(256);
    test(1000);
    test(1024);
    test(1000000);
    assert(globalMemCounter.checkOutstandingNewEq(0));

  return 0;
}
