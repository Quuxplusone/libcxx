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
#include <cassert>

int main()
{
    std::experimental::pmr::memory_resource *expected = std::experimental::pmr::null_memory_resource();
    std::experimental::pmr::set_default_resource(expected);
    {
        char buffer[16];
        std::experimental::pmr::monotonic_buffer_resource r1;
        std::experimental::pmr::monotonic_buffer_resource r2(16);
        std::experimental::pmr::monotonic_buffer_resource r3(buffer, sizeof buffer);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
        assert(r3.upstream_resource() == expected);
    }

    expected = std::experimental::pmr::new_delete_resource();
    std::experimental::pmr::set_default_resource(expected);
    {
        char buffer[16];
        std::experimental::pmr::monotonic_buffer_resource r1;
        std::experimental::pmr::monotonic_buffer_resource r2(16);
        std::experimental::pmr::monotonic_buffer_resource r3(buffer, sizeof buffer);
        assert(r1.upstream_resource() == expected);
        assert(r2.upstream_resource() == expected);
        assert(r3.upstream_resource() == expected);
    }
}
