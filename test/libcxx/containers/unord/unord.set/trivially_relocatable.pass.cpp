//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <unordered_set>
#include <memory>
#include <type_traits>

#if !__has_extension(trivially_relocatable)
#error "oops"
#endif

struct NonTriv { ~NonTriv(); };
template <> struct std::hash<NonTriv> { size_t operator()(const NonTriv&) const; };
bool operator==(const NonTriv&, const NonTriv&);

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

    template<class T> struct unordered_set : std::unordered_set<T, std::hash<T>, std::equal_to<T>, FakeAllocator<T> > {};
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

    template<class T> struct unordered_set : std::unordered_set<T, std::hash<T>, std::equal_to<T>, FakeAllocator<T> > {};
}

namespace fa3 {
    template<class T>
    struct FakePointer {
        int offset;
        FakePointer();
        FakePointer(const FakePointer&) = default;
        FakePointer& operator=(const FakePointer&) = default;
        typename std::conditional<std::is_void<T>::value, char, T>::type& operator*() const;
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

    template<class T> struct unordered_set : std::unordered_set<T, std::hash<T>, std::equal_to<T>, FakeAllocator<T> > {};
}

namespace fa4 {
    template<class T>
    struct FakePointer {
        int offset;
        FakePointer();
        FakePointer(const FakePointer&);
        FakePointer& operator=(const FakePointer&);
        typename std::conditional<std::is_void<T>::value, char, T>::type& operator*() const;
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

    template<class T> struct unordered_set : std::unordered_set<T, std::hash<T>, std::equal_to<T>, FakeAllocator<T> > {};
}

namespace fa5 {
    template<class T>
    struct FakeHasher : std::hash<T> {
        ~FakeHasher();
    };
    static_assert(!std::is_trivially_relocatable<FakeHasher<int> >::value, "");

    template<class T> struct unordered_set : std::unordered_set<T, FakeHasher<T>, std::equal_to<T>, std::allocator<T> > {};
}

namespace fa6 {
    template<class T>
    struct FakeEqualTo : std::equal_to<T> {
        ~FakeEqualTo();
    };
    static_assert(!std::is_trivially_relocatable<FakeEqualTo<int> >::value, "");

    template<class T> struct unordered_set : std::unordered_set<T, std::hash<T>, FakeEqualTo<T>, std::allocator<T> > {};
}

static_assert(std::is_trivially_relocatable< std::allocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa2::FakeAllocator<int> >::value, "");
static_assert(std::is_trivially_relocatable< fa4::FakeAllocator<int> >::value, "");
static_assert(!std::is_trivially_relocatable< fa4::FakePointer<int> >::value, "");

template<class VT, bool Expected>
void check_trivial_relocatability()
{
#if _LIBCPP_DEBUG_LEVEL >= 2
    // Debugging adds significant operations to unordered_set's move-constructor and destructor.
    static_assert(std::is_trivially_relocatable< VT >::value == false, "");
#else
    static_assert(std::is_trivially_relocatable< VT >::value == Expected, "");
#endif
}

int main()
{
    check_trivial_relocatability< std::unordered_set<int>, true >();
    check_trivial_relocatability< std::unordered_set<NonTriv>, true >();

    check_trivial_relocatability< fa::unordered_set<int>, true >();
    check_trivial_relocatability< fa::unordered_set<NonTriv>, true >();

    check_trivial_relocatability< fa2::unordered_set<int>, false >();
    check_trivial_relocatability< fa2::unordered_set<NonTriv>, false >();

    check_trivial_relocatability< fa3::unordered_set<int>, true >();
    check_trivial_relocatability< fa3::unordered_set<NonTriv>, true >();

    check_trivial_relocatability< fa4::unordered_set<int>, false >();
    check_trivial_relocatability< fa4::unordered_set<NonTriv>, false >();

    check_trivial_relocatability< fa5::unordered_set<int>, false >();
    check_trivial_relocatability< fa5::unordered_set<NonTriv>, false >();

    check_trivial_relocatability< fa6::unordered_set<int>, false >();
    check_trivial_relocatability< fa6::unordered_set<NonTriv>, false >();
}
