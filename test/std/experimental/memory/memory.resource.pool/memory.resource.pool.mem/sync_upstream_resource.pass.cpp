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
    std::experimental::pmr::synchronized_pool_resource sync;
    LIBCPP_ASSERT_NOEXCEPT(sync.upstream_resource());

    return 0;
}
