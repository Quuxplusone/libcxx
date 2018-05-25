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
    // Constructing a monotonic_buffer_resource should not cause allocations
    // by itself; the resource should wait to allocate until an allocation is
    // requested.

    globalMemCounter.reset();
    std::experimental::pmr::set_default_resource(std::experimental::pmr::new_delete_resource());

    std::experimental::pmr::monotonic_buffer_resource r1;
    assert(globalMemCounter.checkNewCalledEq(0));

    std::experimental::pmr::monotonic_buffer_resource r2(1024);
    assert(globalMemCounter.checkNewCalledEq(0));

    std::experimental::pmr::monotonic_buffer_resource r3(1024, std::experimental::pmr::new_delete_resource());
    assert(globalMemCounter.checkNewCalledEq(0));

  return 0;
}
