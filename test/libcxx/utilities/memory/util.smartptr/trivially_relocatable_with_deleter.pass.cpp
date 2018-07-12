//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <memory>

// template<class T, class D> class unique_ptr;

#include <memory>
#include <type_traits>

// Define the P1144 name for convenience.
#define is_trivially_relocatable __libcpp_is_trivially_relocatable


struct Incomplete;
struct NonTriv { ~NonTriv(); };

namespace fa {
    template<class T>
    struct FakeDeleter {
        using pointer = T*;
        void operator()(pointer p) const;
    };
    static_assert(std::is_trivially_copy_constructible<FakeDeleter<int>>::value, "");
    static_assert(std::is_trivially_relocatable<FakeDeleter<int>>::value, "");

    template<class T> using unique_ptr = std::unique_ptr<T, FakeDeleter<T>>;
}

namespace fa2 {
    template<class T>
    struct FakeDeleter {
        using pointer = T*;
        ~FakeDeleter();
        void operator()(pointer p) const;
    };
    static_assert(!std::is_trivially_destructible<FakeDeleter<int>>::value, "");
    static_assert(!std::is_trivially_relocatable<FakeDeleter<int>>::value, "");

    template<class T> using unique_ptr = std::unique_ptr<T, FakeDeleter<T>>;
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
    struct FakeDeleter {
        using pointer = FakePointer<T>;
        void operator()(pointer p) const;
    };

    template<class T> using unique_ptr = std::unique_ptr<T, FakeDeleter<T>>;
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
    struct FakeDeleter {
        using pointer = FakePointer<T>;
        void operator()(pointer p) const;
    };

    template<class T> using unique_ptr = std::unique_ptr<T, FakeDeleter<T>>;
}


static_assert(std::is_trivially_relocatable< std::default_delete<int> >::value, "");
static_assert(std::is_trivially_relocatable< std::default_delete<int[]> >::value, "");
static_assert(!std::is_trivially_relocatable< fa2::FakeDeleter<int> >::value, "");
static_assert(std::is_trivially_relocatable< typename fa2::FakeDeleter<int>::pointer >::value, "");
static_assert(std::is_trivially_relocatable< fa4::FakeDeleter<int> >::value, "");
static_assert(!std::is_trivially_relocatable< typename fa4::FakeDeleter<int>::pointer >::value, "");

template<class T, bool Expected>
void check_trivial_relocatability()
{
#if __has_extension(trivially_relocatable)
    static_assert(std::is_trivially_relocatable< T >::value == Expected, "");
#else
    // Making unique_ptr trivially relocatable requires using the attribute.
    static_assert(std::is_trivially_relocatable< T >::value == false, "");
#endif
}

int main()
{
    check_trivial_relocatability< std::unique_ptr<int>, true >();
    check_trivial_relocatability< std::unique_ptr<Incomplete>, true >();
    check_trivial_relocatability< std::unique_ptr<NonTriv>, true >();
    check_trivial_relocatability< std::unique_ptr<int[]>, true >();
    check_trivial_relocatability< std::unique_ptr<Incomplete[]>, true >();
    check_trivial_relocatability< std::unique_ptr<NonTriv[]>, true >();

    check_trivial_relocatability< fa::unique_ptr<int>, true >();
    check_trivial_relocatability< fa::unique_ptr<Incomplete>, true >();
    check_trivial_relocatability< fa::unique_ptr<NonTriv>, true >();
    check_trivial_relocatability< std::unique_ptr<int[]>, true >();
    check_trivial_relocatability< std::unique_ptr<Incomplete[]>, true >();
    check_trivial_relocatability< std::unique_ptr<NonTriv[]>, true >();

    check_trivial_relocatability< fa2::unique_ptr<int>, false >();
    check_trivial_relocatability< fa2::unique_ptr<Incomplete>, false >();
    check_trivial_relocatability< fa2::unique_ptr<NonTriv>, false >();
    check_trivial_relocatability< fa2::unique_ptr<int[]>, false >();
    check_trivial_relocatability< fa2::unique_ptr<Incomplete[]>, false >();
    check_trivial_relocatability< fa2::unique_ptr<NonTriv[]>, false >();

    check_trivial_relocatability< fa3::unique_ptr<int>, true >();
    check_trivial_relocatability< fa3::unique_ptr<Incomplete>, true >();
    check_trivial_relocatability< fa3::unique_ptr<NonTriv>, true >();
    check_trivial_relocatability< fa3::unique_ptr<int[]>, true >();
    check_trivial_relocatability< fa3::unique_ptr<Incomplete[]>, true >();
    check_trivial_relocatability< fa3::unique_ptr<NonTriv[]>, true >();

    check_trivial_relocatability< fa4::unique_ptr<int>, false >();
    check_trivial_relocatability< fa4::unique_ptr<Incomplete>, false >();
    check_trivial_relocatability< fa4::unique_ptr<NonTriv>, false >();
    check_trivial_relocatability< fa4::unique_ptr<int[]>, false >();
    check_trivial_relocatability< fa4::unique_ptr<Incomplete[]>, false >();
    check_trivial_relocatability< fa4::unique_ptr<NonTriv[]>, false >();
}
