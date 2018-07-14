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

// template<class T> class vector;

#include <memory>
#include <vector>
#include <type_traits>

struct Incomplete;
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

    template<class T> using vector = std::vector<T, FakeAllocator<T>>;
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

    template<class T> using vector = std::vector<T, FakeAllocator<T>>;
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

    template<class T> using vector = std::vector<T, FakeAllocator<T>>;
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

    template<class T> using vector = std::vector<T, FakeAllocator<T>>;
}


static_assert(std::is_trivially_relocatable< std::allocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa2::FakeAllocator<int> >::value, "");
static_assert(std::is_trivially_relocatable< fa4::FakeAllocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa4::FakePointer<int> >::value, "");

template<class VT, bool Expected>
void check_trivial_relocatability()
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    // Debugging adds significant operations to vector's move-constructor and destructor.
    static_assert(std::is_trivially_relocatable< VT >::value == false, "");
#elif !__has_extension(trivially_relocatable)
    // Making vector trivially relocatable requires using the attribute.
    static_assert(std::is_trivially_relocatable< VT >::value == false, "");
#else
    static_assert(std::is_trivially_relocatable< VT >::value == Expected, "");
#endif
}

int main()
{
    check_trivial_relocatability< std::vector<int>, true >();
    check_trivial_relocatability< std::vector<Incomplete>, true >();
    check_trivial_relocatability< std::vector<NonTriv>, true >();

    check_trivial_relocatability< fa::vector<int>, true >();
    check_trivial_relocatability< fa::vector<Incomplete>, true >();
    check_trivial_relocatability< fa::vector<NonTriv>, true >();

    check_trivial_relocatability< fa2::vector<int>, false >();
    check_trivial_relocatability< fa2::vector<Incomplete>, false >();
    check_trivial_relocatability< fa2::vector<NonTriv>, false >();

    check_trivial_relocatability< fa3::vector<int>, true >();
    check_trivial_relocatability< fa3::vector<Incomplete>, true >();
    check_trivial_relocatability< fa3::vector<NonTriv>, true >();

    check_trivial_relocatability< fa4::vector<int>, false >();
    check_trivial_relocatability< fa4::vector<Incomplete>, false >();
    check_trivial_relocatability< fa4::vector<NonTriv>, false >();
}
