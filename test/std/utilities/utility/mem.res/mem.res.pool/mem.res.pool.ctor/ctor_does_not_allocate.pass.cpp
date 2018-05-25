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
// class unsynchronized_pool_resource

#include <memory_resource>
#include <cassert>

#include "count_new.hpp"

template<class PoolResource>
void test()
{
    // Constructing a pool resource should not cause allocations
    // by itself; the resource should wait to allocate until an
    // allocation is requested.

    globalMemCounter.reset();
    std::pmr::set_default_resource(std::pmr::new_delete_resource());

    PoolResource r1;
    assert(globalMemCounter.checkNewCalledEq(0));

    PoolResource r2(std::pmr::pool_options{ 1024, 2048 });
    assert(globalMemCounter.checkNewCalledEq(0));

    PoolResource r3(std::pmr::pool_options{ 1024, 2048 }, std::pmr::new_delete_resource());
    assert(globalMemCounter.checkNewCalledEq(0));
}

int main()
{
    test<std::pmr::unsynchronized_pool_resource>();
    test<std::pmr::synchronized_pool_resource>();
}
