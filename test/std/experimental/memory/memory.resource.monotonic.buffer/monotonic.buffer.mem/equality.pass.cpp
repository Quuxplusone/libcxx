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

// class monotonic_buffer_resource

#include <experimental/memory_resource>
#include <new>
#include <type_traits>
#include <cassert>

struct assert_on_compare : public std::experimental::pmr::memory_resource
{
protected:
    void *do_allocate(size_t, size_t)
    { assert(false); }

    void do_deallocate(void *, size_t, size_t)
    { assert(false); }

    bool do_is_equal(std::experimental::pmr::memory_resource const &) const noexcept
    { assert(false); }
};

int main(int, char**)
{
    // Same object
    {
        std::experimental::pmr::monotonic_buffer_resource r1;
        std::experimental::pmr::monotonic_buffer_resource r2;
        assert(r1 == r1);
        assert(r1 != r2);

        std::experimental::pmr::memory_resource & p1 = r1;
        std::experimental::pmr::memory_resource & p2 = r2;
        assert(p1 == p1);
        assert(p1 != p2);
        assert(p1 == r1);
        assert(r1 == p1);
        assert(p1 != r2);
        assert(r2 != p1);
    }
    // Different types
    {
        std::experimental::pmr::monotonic_buffer_resource mono1;
        std::experimental::pmr::memory_resource & r1 = mono1;
        assert_on_compare c;
        std::experimental::pmr::memory_resource & r2 = c;
        assert(r1 != r2);
        assert(!(r1 == r2));
    }

  return 0;
}
