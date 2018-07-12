//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <unordered_map>

#include <unordered_map>
#include <memory>
#include <type_traits>

// Define the P1144 name for convenience.
#define is_trivially_relocatable __libcpp_is_trivially_relocatable


struct NonTriv { ~NonTriv(); };
template <> struct std::hash<NonTriv> { size_t operator()(const NonTriv&) const; };
bool operator==(const NonTriv&, const NonTriv&);

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

    template<class K, class V> using alloc = FakeAllocator<std::pair<const K, V>>;
    template<class K, class V> using unordered_map = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, alloc<K, V>>;
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

    template<class K, class V> using alloc = FakeAllocator<std::pair<const K, V>>;
    template<class K, class V> using unordered_map = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, alloc<K, V>>;
}

namespace fa3 {
    template<class T>
    struct FakePointer {
        int offset;
        FakePointer();
        FakePointer(const FakePointer&) = default;
        FakePointer& operator=(const FakePointer&) = default;
        std::conditional_t<std::is_void<T>::value, char, T>& operator*() const;
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

    template<class K, class V> using alloc = FakeAllocator<std::pair<const K, V>>;
    template<class K, class V> using unordered_map = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, alloc<K, V>>;
}

namespace fa4 {
    template<class T>
    struct FakePointer {
        int offset;
        FakePointer();
        FakePointer(const FakePointer&);
        FakePointer& operator=(const FakePointer&);
        std::conditional_t<std::is_void<T>::value, char, T>& operator*() const;
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

    template<class K, class V> using alloc = FakeAllocator<std::pair<const K, V>>;
    template<class K, class V> using unordered_map = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, alloc<K, V>>;
}

namespace fa5 {
    template<class T>
    struct FakeHasher : std::hash<T> {
        ~FakeHasher();
    };
    static_assert(!std::is_trivially_relocatable<FakeHasher<int>>::value, "");

    template<class K, class V> using unordered_map = std::unordered_map<K, V, FakeHasher<K>, std::equal_to<K>>;
}

namespace fa6 {
    template<class T>
    struct FakeEqualTo : std::equal_to<T> {
        ~FakeEqualTo();
    };
    static_assert(!std::is_trivially_relocatable<FakeEqualTo<int>>::value, "");

    template<class K, class V> using unordered_map = std::unordered_map<K, V, std::hash<K>, FakeEqualTo<K>>;
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
#elif !__has_extension(trivially_relocatable)
    // Making unordered_set trivially relocatable requires using the attribute.
    static_assert(std::is_trivially_relocatable< VT >::value == false, "");
#else
    static_assert(std::is_trivially_relocatable< VT >::value == Expected, "");
#endif
}

int main()
{
    check_trivial_relocatability< std::unordered_map<int, int>, true >();
    check_trivial_relocatability< std::unordered_map<NonTriv, int>, true >();

    check_trivial_relocatability< fa::unordered_map<int, int>, true >();
    check_trivial_relocatability< fa::unordered_map<NonTriv, int>, true >();

    check_trivial_relocatability< fa2::unordered_map<int, int>, false >();
    check_trivial_relocatability< fa2::unordered_map<NonTriv, int>, false >();

    check_trivial_relocatability< fa3::unordered_map<int, int>, true >();
    check_trivial_relocatability< fa3::unordered_map<NonTriv, int>, true >();

    check_trivial_relocatability< fa4::unordered_map<int, int>, false >();
    check_trivial_relocatability< fa4::unordered_map<NonTriv, int>, false >();

    check_trivial_relocatability< fa5::unordered_map<int, int>, false >();
    check_trivial_relocatability< fa5::unordered_map<NonTriv, int>, false >();

    check_trivial_relocatability< fa6::unordered_map<int, int>, false >();
    check_trivial_relocatability< fa6::unordered_map<NonTriv, int>, false >();
}
