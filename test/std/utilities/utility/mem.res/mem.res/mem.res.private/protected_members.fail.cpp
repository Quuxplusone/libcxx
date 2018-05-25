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

// monotonic_buffer_resource::do_allocate(size_t, size_t);          /* protected */
// monotonic_buffer_resource::do_deallocate(void*, size_t, size_t); /* protected */
// monotonic_buffer_resource::do_is_equal(memory_resource const&);  /* protected */

// synchronized_pool_resource::do_allocate(size_t, size_t);          /* protected */
// synchronized_pool_resource::do_deallocate(void*, size_t, size_t); /* protected */
// synchronized_pool_resource::do_is_equal(memory_resource const&);  /* protected */

// unsynchronized_pool_resource::do_allocate(size_t, size_t);          /* protected */
// unsynchronized_pool_resource::do_deallocate(void*, size_t, size_t); /* protected */
// unsynchronized_pool_resource::do_is_equal(memory_resource const&);  /* protected */

#include <memory_resource>

int main() {
    {
        std::pmr::monotonic_buffer_resource m;
        m.do_allocate(0, 0); // expected-error{{'do_allocate' is a protected member}}
        m.do_deallocate(nullptr, 0, 0); // expected-error{{'do_deallocate' is a protected member}}
        m.do_is_equal(m); // expected-error{{'do_is_equal' is a protected member}}
    }
    {
        std::pmr::synchronized_pool_resource m;
        m.do_allocate(0, 0); // expected-error{{'do_allocate' is a protected member}}
        m.do_deallocate(nullptr, 0, 0); // expected-error{{'do_deallocate' is a protected member}}
        m.do_is_equal(m); // expected-error{{'do_is_equal' is a protected member}}
    }
    {
        std::pmr::unsynchronized_pool_resource m;
        m.do_allocate(0, 0); // expected-error{{'do_allocate' is a protected member}}
        m.do_deallocate(nullptr, 0, 0); // expected-error{{'do_deallocate' is a protected member}}
        m.do_is_equal(m); // expected-error{{'do_is_equal' is a protected member}}
    }
}
