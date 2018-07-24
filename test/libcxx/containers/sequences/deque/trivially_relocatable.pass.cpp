//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <deque>

// template<class T> class deque;

#include <deque>
#include <memory>
#include <type_traits>

struct NonTriv { ~NonTriv(); };

namespace fa {
    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            using other = FakeAllocator<U>;
        };
        template<class U>
        FakeAllocator(const FakeAllocator<U>&);
        // The defaulted (trivial) copy constructor is still present here!
    };
    static_assert(std::is_trivially_copy_constructible<FakeAllocator<int>>::value, "");
    static_assert(std::is_trivially_relocatable<FakeAllocator<int>>::value, "");

    template<class T> using deque = std::deque<T, FakeAllocator<T>>;
}

namespace fa2 {
    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            using other = FakeAllocator<U>;
        };
        template<class U>
        FakeAllocator(const FakeAllocator<U>&);
        FakeAllocator(const FakeAllocator&);
    };
    static_assert(!std::is_trivially_copy_constructible<FakeAllocator<int>>::value, "");
    static_assert(!std::is_trivially_relocatable<FakeAllocator<int>>::value, "");

    template<class T> using deque = std::deque<T, FakeAllocator<T>>;
}

namespace fa3 {
    template<class T>
    struct FakePointer {
        int offset;
        FakePointer();
        FakePointer(const FakePointer&) = default;
        FakePointer& operator=(const FakePointer&) = default;
        T& operator*() const;
        T* operator->() const;
        FakePointer operator + (unsigned int) const;
        FakePointer& operator += (unsigned int);
        FakePointer operator ++(int);
        FakePointer& operator ++();
        FakePointer operator --(int);
        FakePointer& operator --();
    };
    static_assert(std::is_trivially_copy_constructible<FakePointer<int>>::value, "");
    static_assert(std::is_trivially_relocatable<FakePointer<int>>::value, "");

    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            using other = FakeAllocator<U>;
        };
        using pointer = FakePointer<T>;
    };

    template<class T> using deque = std::deque<T, FakeAllocator<T>>;
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
    static_assert(!std::is_trivially_copy_constructible<FakePointer<int>>::value, "");
    static_assert(!std::is_trivially_relocatable<FakePointer<int>>::value, "");

    template<class T>
    struct FakeAllocator : std::allocator<T> {
        template<class U> struct rebind {
            using other = FakeAllocator<U>;
        };
        using pointer = FakePointer<T>;
    };

    template<class T> using deque = std::deque<T, FakeAllocator<T>>;
}


static_assert(std::is_trivially_relocatable< std::allocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa2::FakeAllocator<int> >::value, "");
static_assert(std::is_trivially_relocatable< fa4::FakeAllocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa4::FakePointer<int> >::value, "");

template<class VT, bool Expected>
void check_trivial_relocatability()
{
#if !__has_extension(trivially_relocatable)
    // Making deque trivially relocatable requires using the attribute.
    static_assert(std::is_trivially_relocatable< VT >::value == false, "");
#else
    static_assert(std::is_trivially_relocatable< VT >::value == Expected, "");
#endif
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
