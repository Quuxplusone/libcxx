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

#include "count_new.h"

int main(int, char**)
{
    globalMemCounter.reset();
    std::pmr::monotonic_buffer_resource mono;

    for (int i=0; i < 100; ++i) {
        mono.allocate(1);
        assert(globalMemCounter.last_new_size < 1000000000);
        mono.release();
        assert(globalMemCounter.checkOutstandingNewEq(0));
    }

  return 0;
}
