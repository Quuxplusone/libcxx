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

// class unsynchronized_pool_resource

#include <memory_resource>
#include <new>
#include <type_traits>
#include <cassert>

#include "count_new.hpp"

struct assert_on_compare : public std::pmr::memory_resource
{
protected:
    virtual void * do_allocate(size_t, size_t)
    { assert(false); }

    virtual void do_deallocate(void *, size_t, size_t)
    { assert(false); }

    virtual bool do_is_equal(std::pmr::memory_resource const &) const noexcept
    { assert(false); }
};

static bool is_aligned_to(void *p, size_t alignment)
{
    void *p2 = p;
    size_t space = 1;
    void *result = std::align(alignment, 1, p2, space);
    return (result == p);
}

void test_construction_with_default_resource()
{
    std::pmr::memory_resource *expected = std::pmr::null_memory_resource();
    std::pmr::set_default_resource(expected);
    {
        std::pmr::pool_options opts { 0, 0 };
        std::pmr::unsynchronized_pool_resource r1;
        std::pmr::unsynchronized_pool_resource r2(opts);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
    }

    expected = std::pmr::new_delete_resource();
    std::pmr::set_default_resource(expected);
    {
        std::pmr::pool_options opts { 1024, 2048 };
        std::pmr::unsynchronized_pool_resource r1;
        std::pmr::unsynchronized_pool_resource r2(opts);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
    }
}

void test_construction_does_not_allocate()
{
    // Constructing a unsynchronized_pool_resource should not cause allocations
    // by itself; the resource should wait to allocate until an allocation is
    // requested.

    globalMemCounter.reset();
    std::pmr::set_default_resource(std::pmr::new_delete_resource());

    std::pmr::unsynchronized_pool_resource r1;
    assert(globalMemCounter.checkNewCalledEq(0));

    std::pmr::unsynchronized_pool_resource r2(std::pmr::pool_options{ 1024, 2048 });
    assert(globalMemCounter.checkNewCalledEq(0));

    std::pmr::unsynchronized_pool_resource r3(std::pmr::pool_options{ 1024, 2048 }, std::pmr::new_delete_resource());
    assert(globalMemCounter.checkNewCalledEq(0));
}

void test_equality()
{
    // Same object
    {
        std::pmr::unsynchronized_pool_resource r1;
        std::pmr::unsynchronized_pool_resource r2;
        assert(r1 == r1);
        assert(r1 != r2);

        std::pmr::memory_resource & p1 = r1;
        std::pmr::memory_resource & p2 = r2;
        assert(p1 == p1);
        assert(p1 != p2);
        assert(p1 == r1);
        assert(r1 == p1);
        assert(p1 != r2);
        assert(r2 != p1);
    }
    // Different types
    {
        std::pmr::unsynchronized_pool_resource unsync1;
        std::pmr::memory_resource & r1 = unsync1;
        assert_on_compare c;
        std::pmr::memory_resource & r2 = c;
        assert(r1 != r2);
        assert(!(r1 == r2));
    }
}

void test_allocate_deallocate()
{
    {
        globalMemCounter.reset();

        std::pmr::unsynchronized_pool_resource unsync1(std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = unsync1;

        void *ret = r1.allocate(50);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        r1.deallocate(ret, 50);
        unsync1.release();
        assert(globalMemCounter.checkDeleteCalledGreaterThan(0));
        assert(globalMemCounter.checkOutstandingNewEq(0));

        globalMemCounter.reset();

        ret = r1.allocate(500);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        // Check that the destructor calls release()
    }
    assert(globalMemCounter.checkDeleteCalledGreaterThan(0));
    assert(globalMemCounter.checkOutstandingNewEq(0));
}

void test_overaligned_single_allocation()
{
    globalMemCounter.reset();
    std::pmr::pool_options opts { 1, 1024 };
    std::pmr::unsynchronized_pool_resource unsync1(opts, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = unsync1;

    constexpr size_t big_alignment = 8 * alignof(std::max_align_t);
    static_assert(big_alignment > 4);

    assert(globalMemCounter.checkNewCalledEq(0));

    void *ret = r1.allocate(2048, big_alignment);
    assert(ret != nullptr);
    assert(is_aligned_to(ret, big_alignment));
    assert(globalMemCounter.checkNewCalledGreaterThan(0));

    ret = r1.allocate(16, 4);
    assert(ret != nullptr);
    assert(is_aligned_to(ret, 4));
    assert(globalMemCounter.checkNewCalledGreaterThan(1));
}

void test_reuse()
{
    globalMemCounter.reset();
    std::pmr::pool_options opts { 1, 256 };
    std::pmr::unsynchronized_pool_resource unsync1(opts, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = unsync1;

    void *ret = r1.allocate(8);
    assert(ret != nullptr);
    assert(is_aligned_to(ret, 8));
    assert(globalMemCounter.checkNewCalledGreaterThan(0));
    int new_called = globalMemCounter.new_called;

    // After deallocation, the pool for 8-byte blocks should have at least one vacancy.
    r1.deallocate(ret, 8);
    assert(globalMemCounter.new_called == new_called);
    assert(globalMemCounter.checkDeleteCalledEq(0));

    // This should return an existing block from the pool: no new allocations.
    ret = r1.allocate(8);
    assert(ret != nullptr);
    assert(is_aligned_to(ret, 8));
    assert(globalMemCounter.new_called == new_called);
    assert(globalMemCounter.checkDeleteCalledEq(0));
}

int main()
{
    test_construction_with_default_resource();
    test_construction_does_not_allocate();
    test_equality();
    test_allocate_deallocate();
    test_overaligned_single_allocation();
    test_reuse();
}
