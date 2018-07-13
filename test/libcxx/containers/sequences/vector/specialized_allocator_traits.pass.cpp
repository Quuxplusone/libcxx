//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <vector>

// Test that vector does not use non-standard members of std::allocator_traits.
// Specializing std::allocator_traits is arguably non-conforming, but libc++'s
// support for specialized std::allocator_traits is a feature, not a bug.
// Breaking (and subsequently deleting) this unit test should be done as a
// conscious decision.

#include <vector>

template <class T>
class A1
{
public:
    using value_type = T;

    A1() = default;

    template <class U>
    A1(const A1<U>&) {}

    T *allocate(std::size_t n)
    {
        return (T *)std::malloc(n * sizeof (T));
    }

    void deallocate(T* p, std::size_t)
    {
        std::free(p);
    }
};

template<class T>
struct std::allocator_traits<A1<T>> {
    using allocator_type = A1<T>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::true_type;

    template<class U> using rebind_alloc = A1<U>;
    template<class U> using rebind_traits = std::allocator_traits<A1<U>>;

    static T *allocate(A1<T>& a, size_t n) {
        return a.allocate(n);
    }

    static void deallocate(A1<T>& a, T *p, size_t n) {
        return a.deallocate(p, n);
    }

    template<class U, class... Args>
    static void construct(A1<T>&, U *p, Args&&... args) {
        ::new ((void*)p) U(std::forward<Args>(args)...);
    }

    template<class U>
    static void destroy(A1<T>&, U *p) {
        p->~U();
    }

    static A1<T> select_on_container_copy_construction(const A1<T>& a) {
        return a.select_on_container_copy_construction();
    }

    static size_type max_size(const A1<T>&) {
        return size_t(-1);
    }
};

int main()
{
    std::vector<int, A1<int>> v = {1, 2, 3};
    v.resize(10);
    v.insert(v.begin() + 4, 4);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    assert(v[3] == 0);
    assert(v[4] == 4);
    assert(v[5] == 0);
}
