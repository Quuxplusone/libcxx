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
// class unsynchronized_pool_resource

#include <experimental/memory_resource>
#include <new>
#include <type_traits>
#include <cassert>

#include "count_new.hpp"

class assert_on_compare : public std::experimental::pmr::memory_resource
{
    void * do_allocate(size_t, size_t) override
    { assert(false); }

    void do_deallocate(void *, size_t, size_t) override
    { assert(false); }

    bool do_is_equal(std::experimental::pmr::memory_resource const &) const noexcept override
    { assert(false); }
};

template<class PoolResource>
void test()
{
    // Same type
    {
        PoolResource pr1;
        PoolResource pr2;
        assert(pr1 == pr1);
        assert(pr1 != pr2);

        std::experimental::pmr::memory_resource & mr1 = pr1;
        std::experimental::pmr::memory_resource & mr2 = pr2;
        assert(mr1 == mr1);
        assert(mr1 != mr2);
        assert(mr1 == pr1);
        assert(pr1 == mr1);
        assert(mr1 != pr2);
        assert(pr2 != mr1);
    }
    // Different types
    {
        PoolResource pr1;
        std::experimental::pmr::memory_resource & mr1 = pr1;
        assert_on_compare c;
        std::experimental::pmr::memory_resource & mr2 = c;
        assert(mr1 != mr2);
        assert(!(mr1 == mr2));
    }
}

int main()
{
    test<std::experimental::pmr::synchronized_pool_resource>();
    test<std::experimental::pmr::unsynchronized_pool_resource>();
}
