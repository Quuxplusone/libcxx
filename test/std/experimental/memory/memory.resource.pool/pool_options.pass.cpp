//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// REQUIRES: c++experimental
// UNSUPPORTED: c++98, c++03, c++11, c++14
// UNSUPPORTED: apple-clang-7

// <experimental/memory_resource>

// struct pool_options

#include <cassert>
#include <experimental/memory_resource>

int main(int, char**)
{
    const std::experimental::pmr::pool_options p;
    assert(p.max_blocks_per_chunk == 0);
    assert(p.largest_required_pool_block == 0);

    return 0;
}
