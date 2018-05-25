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

// class unsynchronized_pool_resource

#include <experimental/memory_resource>
#include <cassert>

#include "count_new.hpp"

int main(int, char**)
{
    std::experimental::pmr::unsynchronized_pool_resource unsync;
    LIBCPP_ASSERT_NOEXCEPT(unsync.options());

    return 0;
}
