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

// class synchronized_pool_resource

#include <memory_resource>
#include <cassert>

int main()
{
    std::pmr::memory_resource *expected = std::pmr::null_memory_resource();
    std::pmr::set_default_resource(expected);
    {
        std::pmr::pool_options opts { 0, 0 };
        std::pmr::synchronized_pool_resource r1;
        std::pmr::synchronized_pool_resource r2(opts);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
    }

    expected = std::pmr::new_delete_resource();
    std::pmr::set_default_resource(expected);
    {
        std::pmr::pool_options opts { 1024, 2048 };
        std::pmr::synchronized_pool_resource r1;
        std::pmr::synchronized_pool_resource r2(opts);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
    }
}
