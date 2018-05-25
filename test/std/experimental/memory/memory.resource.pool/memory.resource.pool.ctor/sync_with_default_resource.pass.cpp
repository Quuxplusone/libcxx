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

int main(int, char**)
{
    std::experimental::pmr::memory_resource *expected = std::experimental::pmr::null_memory_resource();
    std::experimental::pmr::set_default_resource(expected);
    {
        std::experimental::pmr::pool_options opts { 0, 0 };
        std::experimental::pmr::synchronized_pool_resource r1;
        std::experimental::pmr::synchronized_pool_resource r2(opts);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
    }

    expected = std::experimental::pmr::new_delete_resource();
    std::experimental::pmr::set_default_resource(expected);
    {
        std::experimental::pmr::pool_options opts { 1024, 2048 };
        std::experimental::pmr::synchronized_pool_resource r1;
        std::experimental::pmr::synchronized_pool_resource r2(opts);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
    }

    return 0;
}
