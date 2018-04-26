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

void test_construction_with_default_resource()
{
    std::pmr::memory_resource *expected = std::pmr::null_memory_resource();
    std::pmr::set_default_resource(expected);
    {
        char buffer[16];
        std::pmr::monotonic_buffer_resource r1;
        std::pmr::monotonic_buffer_resource r2(16);
        std::pmr::monotonic_buffer_resource r3(buffer, sizeof buffer);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
        assert(r3.upstream_resource() == expected);
    }

    expected = std::pmr::new_delete_resource();
    std::pmr::set_default_resource(expected);
    {
        char buffer[16];
        std::pmr::monotonic_buffer_resource r1;
        std::pmr::monotonic_buffer_resource r2(16);
        std::pmr::monotonic_buffer_resource r3(buffer, sizeof buffer);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
        assert(r3.upstream_resource() == expected);
    }
}

void test_construction_without_buffer()
{
    // Constructing a monotonic_buffer_resource should not cause allocations
    // by itself; the resource should wait to allocate until an allocation is
    // requested.

    globalMemCounter.reset();
    std::pmr::set_default_resource(std::pmr::new_delete_resource());

    std::pmr::monotonic_buffer_resource r1;
    assert(globalMemCounter.checkNewCalledEq(0));

    std::pmr::monotonic_buffer_resource r2(1024);
    assert(globalMemCounter.checkNewCalledEq(0));

    std::pmr::monotonic_buffer_resource r3(1024, std::pmr::new_delete_resource());
    assert(globalMemCounter.checkNewCalledEq(0));
}

void test_equality()
{
    // Same object
    {
        std::pmr::monotonic_buffer_resource r1;
        std::pmr::monotonic_buffer_resource r2;
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
        std::pmr::monotonic_buffer_resource mono1;
        std::pmr::memory_resource & r1 = mono1;
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

        std::pmr::monotonic_buffer_resource mono1(std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(50);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        // mem.res.monotonic.buffer 1.2
        // A call to deallocate has no effect, thus the amount of memory
        // consumed increases monotonically until the resource is destroyed.
        // Check that deallocate is a no-op
        r1.deallocate(ret, 50);
        assert(globalMemCounter.checkDeleteCalledEq(0));

        mono1.release();
        assert(globalMemCounter.checkDeleteCalledEq(1));
        assert(globalMemCounter.checkOutstandingNewEq(0));

        globalMemCounter.reset();

        ret = r1.allocate(500);
        assert(ret);
        assert(globalMemCounter.checkNewCalledGreaterThan(0));
        assert(globalMemCounter.checkDeleteCalledEq(0));

        // Check that the destructor calls release()
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));
    assert(globalMemCounter.checkOutstandingNewEq(0));
}

void test_allocate_deallocate_from_original_buffer()
{
    globalMemCounter.reset();
    char buffer[100];
    std::pmr::monotonic_buffer_resource mono1((void *)buffer, sizeof buffer, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = mono1;

    // Check that construction with a buffer does not allocate anything from the upstream
    assert(globalMemCounter.checkNewCalledEq(0));

    // Check that an allocation that fits in the buffer does not allocate anything from the upstream
    void *ret = r1.allocate(50);
    assert(ret);
    assert(globalMemCounter.checkNewCalledEq(0));

    // Check a second allocation
    ret = r1.allocate(20);
    assert(ret);
    assert(globalMemCounter.checkNewCalledEq(0));

    r1.deallocate(ret, 50);
    assert(globalMemCounter.checkDeleteCalledEq(0));

    // Check an allocation that doesn't fit in the original buffer
    ret = r1.allocate(50);
    assert(ret);
    assert(globalMemCounter.checkNewCalledEq(1));

    r1.deallocate(ret, 50);
    assert(globalMemCounter.checkDeleteCalledEq(0));

    mono1.release();
    assert(globalMemCounter.checkDeleteCalledEq(1));
    assert(globalMemCounter.checkOutstandingNewEq(0));
}

void test_geometric_progression()
{
    // mem.res.monotonic.buffer 1.3
    // Each additional buffer is larger than the previous one, following a
    // geometric progression.

    globalMemCounter.reset();
    std::pmr::monotonic_buffer_resource mono1(100, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = mono1;

    assert(globalMemCounter.checkNewCalledEq(0));
    size_t next_buffer_size = 100;
    void *ret = r1.allocate(10, 1);
    assert(ret != nullptr);
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.last_new_size >= next_buffer_size);
    next_buffer_size = globalMemCounter.last_new_size + 1;

    int new_called = 1;
    while (new_called < 5) {
        ret = r1.allocate(10, 1);
        if (globalMemCounter.new_called > new_called) {
            assert(globalMemCounter.new_called == new_called + 1);
            assert(globalMemCounter.last_new_size >= next_buffer_size);
            next_buffer_size = globalMemCounter.last_new_size + 1;
            new_called += 1;
        }
    }
}

void test_geometric_progression_after_release()
{
    // mem.res.monotonic.buffer 1.3
    // Each additional buffer is larger than the previous one, following a
    // geometric progression.

    // mem.res.monotonic.buffer.mem 1
    // release() calls upstream_rsrc->deallocate() as necessary to release all allocated memory.

    // Implicitly: release() does not reset the geometric progression of next_buffer_size.

    globalMemCounter.reset();
    std::pmr::monotonic_buffer_resource mono1;
    std::pmr::memory_resource & r1 = mono1;

    void *ret = r1.allocate(100, 1);
    assert(ret != nullptr);
    assert(globalMemCounter.checkNewCalledEq(1));
    size_t last_new_size = globalMemCounter.last_new_size;

    r1.allocate(last_new_size, 1);
    assert(globalMemCounter.checkNewCalledEq(2));
    assert(globalMemCounter.last_new_size > last_new_size);
    last_new_size = globalMemCounter.last_new_size;

    mono1.release();
    assert(globalMemCounter.checkDeleteCalledEq(2));

    // We expect to see a large upstream allocation corresponding
    // to this small request, because the upstream allocation must
    // be at least `next_buffer_size` bytes.
    r1.allocate(10, 1);
    assert(globalMemCounter.checkNewCalledEq(3));
    assert(globalMemCounter.last_new_size >= 10);
    assert(globalMemCounter.last_new_size > 100);
    assert(globalMemCounter.last_new_size > last_new_size);
}

void test_zero_sized_initial_buffer()
{
    globalMemCounter.reset();
    {
        char buffer[100];
        std::pmr::monotonic_buffer_resource mono1((void *)buffer, 0, std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret != nullptr);
        assert(globalMemCounter.checkNewCalledEq(1));
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));

    globalMemCounter.reset();
    {
        std::pmr::monotonic_buffer_resource mono1(nullptr, 0, std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret != nullptr);
        assert(globalMemCounter.checkNewCalledEq(1));
    }
    assert(globalMemCounter.checkDeleteCalledEq(1));
}

void test_underaligned_initial_buffer()
{
    globalMemCounter.reset();
    {
        alignas(4) char buffer[17];
        std::pmr::monotonic_buffer_resource mono1(buffer + 1, 16, std::pmr::new_delete_resource());
        std::pmr::memory_resource & r1 = mono1;

        void *ret = r1.allocate(1, 1);
        assert(ret == buffer + 1);
        mono1.release();

        ret = r1.allocate(1, 2);
        assert(ret == buffer + 2);
        mono1.release();

        ret = r1.allocate(1, 4);
        assert(ret == buffer + 4);
        mono1.release();

        // Test a size that is just big enough to fit in the buffer,
        // but can't fit if it's aligned.
        ret = r1.allocate(16, 1);
        assert(ret == buffer + 1);
        mono1.release();

        assert(globalMemCounter.checkNewCalledEq(0));
        ret = r1.allocate(16, 2);
        assert(globalMemCounter.checkNewCalledEq(1));
        assert(globalMemCounter.last_new_size >= 16);
        // assert(globalMemCounter.last_new_align >= 2);
        mono1.release();
        assert(globalMemCounter.checkDeleteCalledEq(1));
        // assert(globalMemCounter.last_new_align == globalMemCounter.last_delete_align);
    }
}

void test_overaligned_single_allocation()
{
    globalMemCounter.reset();
    std::pmr::monotonic_buffer_resource mono1(1024, std::pmr::new_delete_resource());
    std::pmr::memory_resource & r1 = mono1;

    constexpr size_t big_alignment = 8 * alignof(std::max_align_t);
    static_assert(big_alignment > 4);

    void *ret = r1.allocate(2048, big_alignment);
    assert(ret != nullptr);
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.last_new_size >= 2048);
    // assert(globalMemCounter.last_new_align >= big_alignment);

    // Check that a single highly aligned allocation request doesn't
    // permanently "poison" the resource to allocate only super-aligned
    // blocks of memory.
    ret = r1.allocate(globalMemCounter.last_new_size, 4);
    assert(ret != nullptr);
    assert(globalMemCounter.checkNewCalledEq(2));
    // assert(globalMemCounter.last_new_align >= 4);
    // assert(globalMemCounter.last_new_align < big_alignment);
}

int main()
{
    test_construction_with_default_resource();
    test_construction_without_buffer();
    test_equality();
    test_allocate_deallocate();
    test_allocate_deallocate_from_original_buffer();
    test_geometric_progression();
    test_geometric_progression_after_release();
    test_zero_sized_initial_buffer();
    test_underaligned_initial_buffer();
    test_overaligned_single_allocation();
}
