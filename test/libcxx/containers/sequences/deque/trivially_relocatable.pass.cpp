//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <deque>
#include <memory>
#include <type_traits>

#if !__has_extension(trivially_relocatable)
#error "oops"
#endif

struct NonTriv { ~NonTriv(); };

namespace fa {
    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            typedef FakeAllocator<U> other;
        };
        template<class U>
        FakeAllocator(const FakeAllocator<U>&);
        // The defaulted (trivial) copy constructor is still present here!
    };
    static_assert(std::is_trivially_copy_constructible<FakeAllocator<int> >::value, "");
    static_assert(std::is_trivially_relocatable<FakeAllocator<int> >::value, "");

    template<class T> struct deque : std::deque<T, FakeAllocator<T> > {};
}

namespace fa2 {
    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            typedef FakeAllocator<U> other;
        };
        template<class U>
        FakeAllocator(const FakeAllocator<U>&);
        FakeAllocator(const FakeAllocator&);
    };
    static_assert(!std::is_trivially_copy_constructible<FakeAllocator<int> >::value, "");
    static_assert(!std::is_trivially_relocatable<FakeAllocator<int> >::value, "");

    template<class T> struct deque : std::deque<T, FakeAllocator<T> > {};
}

namespace fa3 {
    template<class T>
    struct FakePointer {
        int offset;
        FakePointer();
        // implicitly defaulted copy (and move) operations
        T& operator*() const;
        T* operator->() const;
        FakePointer operator + (unsigned int) const;
        FakePointer& operator += (unsigned int);
        FakePointer operator ++(int);
        FakePointer& operator ++();
        FakePointer operator --(int);
        FakePointer& operator --();
    };
    static_assert(std::is_trivially_copy_constructible<FakePointer<int> >::value, "");
    static_assert(std::is_trivially_relocatable<FakePointer<int> >::value, "");

    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            typedef FakeAllocator<U> other;
        };
        typedef FakePointer<T> pointer;
    };

    template<class T> struct deque : std::deque<T, FakeAllocator<T> > {};
}

namespace fa4 {
    template<class T>
    struct FakePointer {
        int offset;
        FakePointer();
        FakePointer(const FakePointer&);
        FakePointer& operator=(const FakePointer&);
        T& operator*() const;
        T* operator->() const;
        FakePointer operator + (unsigned int) const;
        FakePointer& operator += (unsigned int);
        FakePointer operator ++(int);
        FakePointer& operator ++();
        FakePointer operator --(int);
        FakePointer& operator --();
    };
    static_assert(!std::is_trivially_copy_constructible<FakePointer<int> >::value, "");
    static_assert(!std::is_trivially_relocatable<FakePointer<int> >::value, "");

    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            typedef FakeAllocator<U> other;
        };
        typedef FakePointer<T> pointer;
    };

    template<class T> struct deque : std::deque<T, FakeAllocator<T> > {};
}


static_assert(std::is_trivially_relocatable< std::allocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa2::FakeAllocator<int> >::value, "");
static_assert(std::is_trivially_relocatable< fa4::FakeAllocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa4::FakePointer<int> >::value, "");

template<class VT, bool Expected>
void check_trivial_relocatability()
{
    static_assert(std::is_trivially_relocatable< VT >::value == Expected, "");
}

int main()
{
    check_trivial_relocatability< std::deque<int>, true >();
    check_trivial_relocatability< std::deque<NonTriv>, true >();

    check_trivial_relocatability< fa::deque<int>, true >();
    check_trivial_relocatability< fa::deque<NonTriv>, true >();

    check_trivial_relocatability< fa2::deque<int>, false >();
    check_trivial_relocatability< fa2::deque<NonTriv>, false >();

    check_trivial_relocatability< fa3::deque<int>, true >();
    check_trivial_relocatability< fa3::deque<NonTriv>, true >();

    check_trivial_relocatability< fa4::deque<int>, false >();
    check_trivial_relocatability< fa4::deque<NonTriv>, false >();
}
