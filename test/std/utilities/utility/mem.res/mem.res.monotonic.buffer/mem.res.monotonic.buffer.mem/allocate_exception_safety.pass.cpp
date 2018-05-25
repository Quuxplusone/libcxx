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

struct repointable_resource : public std::pmr::memory_resource
{
    std::pmr::memory_resource *which;

    explicit repointable_resource(std::pmr::memory_resource *res) : which(res) {}

protected:
    virtual void *do_allocate(size_t size, size_t align)
    { return which->allocate(size, align); }

    virtual void do_deallocate(void *p, size_t size, size_t align)
    { return which->deallocate(p, size, align); }

    virtual bool do_is_equal(std::pmr::memory_resource const &rhs) const noexcept
    { return which->is_equal(rhs); }
};

int main()
{
    globalMemCounter.reset();
    repointable_resource upstream(std::pmr::new_delete_resource());
    alignas(16) char buffer[64];
    std::pmr::monotonic_buffer_resource mono1(buffer, sizeof buffer, &upstream);
    std::pmr::memory_resource & r1 = mono1;

    void *res = r1.allocate(64, 16);
    assert(res == buffer);
    assert(globalMemCounter.checkNewCalledEq(0));

    res = r1.allocate(64, 16);
    assert(res != buffer);
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.checkDeleteCalledEq(0));
    const size_t last_new_size = globalMemCounter.last_new_size;

    upstream.which = std::pmr::null_memory_resource();
    try {
        res = r1.allocate(last_new_size, 16);
        assert(false);
    } catch (const std::bad_alloc&) {
        // we expect this
    }
    assert(globalMemCounter.checkNewCalledEq(1));
    assert(globalMemCounter.checkDeleteCalledEq(0));

    upstream.which = std::pmr::new_delete_resource();
    res = r1.allocate(last_new_size, 16);
    assert(res != buffer);
    assert(globalMemCounter.checkNewCalledEq(2));
    assert(globalMemCounter.checkDeleteCalledEq(0));

    mono1.release();
    assert(globalMemCounter.checkNewCalledEq(2));
    assert(globalMemCounter.checkDeleteCalledEq(2));
}
